#include "app.h"
#include "imgui.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <array>
#include <fstream>
#include <nlohmann/json.hpp>
#include <regex>
#include <algorithm>

using json = nlohmann::json;

namespace MyApp {


    //**************************************
    //************  Windows  ***************
    //**************************************
    static bool subject_window = false;
    static bool semester_window = false;
    static bool semester_error_window = false;
    static bool subject_added_window = false;
    static bool subject_rejected_window = false;
    static bool delete_semester_window = false;
    static bool delete_subject_window = false;
    static bool disable_main_window = false;

    ImGuiWindowFlags functional_windows_flags =  ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking;

    //**************************************
    //**********  Variables  ***************
    //**************************************

    struct Sem {
        std::string name; // f.e "Semestr 0"
        std::vector<std::string> subjects;
        Sem(const std::string& n) { name = n; }
    };
    static std::vector<Sem> semy;
    static std::string currentSem = "";  //trackuje ktory semestr jest klikniety w rozwijanej liscie na pasku menu
                                         // trzeba sprobwac zmienic to na iterator tbh powinno byc to lepsze rozwiazanie
                                         // albo i nie peepoGlad

    //**************************************
    //**********  Functions  ***************
    //**************************************


    bool IsSemNumberNotUsed(int number)
    {
        std::stringstream ss;
        for (auto it = semy.begin(); it != semy.end(); it++)
        {
            std::string tmp;
            int x;
            ss << it->name;
            ss >> tmp >> x;
            if (x == number)
                return false;
            ss.clear();
        }
        return true;
    }


    void SortSemsList()
    {
        // Sort semy vector by the last number in name string
        std::sort(semy.begin(), semy.end(), [](const Sem& a, const Sem& b) {
            // Extract last number from name strings using regular expression
            std::regex pattern(R"(\d+)");
            std::smatch match_a, match_b;
            std::regex_search(a.name, match_a, pattern);
            std::regex_search(b.name, match_b, pattern);

            // Convert last number to integers and compare
            return std::stoi(match_a[0]) < std::stoi(match_b[0]);
            });
    }

    void SemesterSubjectsTable(const std::string& currentSem)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf;
        if (ImGui::TreeNodeEx("***Przedmioty w semestrze***", flags))
        {
            for (auto semester : semy)
                if (semester.name == currentSem)
                    for (auto it = semester.subjects.begin(); it != semester.subjects.end(); it++)
                    {
                        ImGui::Text(it->c_str());
                        ImGui::SameLine();
                        ImGui::Button("Dodaj zaliczenie");
                    }
            ImGui::TreePop();
        }
    }

    //Funkcja do wczytania z pliku JSON rzeczy do pamieci programu
    void InitSem()
    {
        // Load 
        std::ifstream input_file("semesters.json");
        json semesters_json;
        input_file >> semesters_json;
        // Iterate
        for (auto& semester : semesters_json) {
            Sem sem(semester["name"]);
            for (auto& subject : semester["subjects"]) {
                sem.subjects.push_back(subject);
            }
            semy.push_back(sem);
        }
    }



    bool DoesSubjectNotExist(std::string nameOfSubject)
    {
        for (auto it = semy.begin(); it != semy.end(); it++)
            if (it->name == currentSem)
                for (auto jt = it->subjects.begin(); jt != it->subjects.end(); jt++)
                    if (*jt == nameOfSubject)
                        return false;
        return true;
    }

    json LoadSemesters()
    {
        std::ifstream infile("semesters.json");
        json j;
        infile >> j;
        return j;
    }

    void SaveSemesters(json& j)
    {
        std::ofstream outfile("semesters.json");
        outfile << std::setw(4) << j << std::endl;
    }

    void AddSubjectToSemester(int semester_index, const std::string& subject_name)
    {
        json semesters = LoadSemesters();
        json& semester = semesters[semester_index];
        semester["subjects"].push_back(subject_name);
        SaveSemesters(semesters);
    }

    void RemoveSubjectFromSemester(int semester_index, const std::string& subject_name)
    {
        json semesters = LoadSemesters();
        json& semester = semesters[semester_index];
        auto it = semester["subjects"].find(subject_name);
        semester["subjects"].erase(it);
        SaveSemesters(semesters);
    }


    // ------------------  WINDOWS  -----------------------

    void DeleteSubjectWindow()
    {
        ImGui::Begin("Usuwanie przedmiotu", NULL, functional_windows_flags);

        ImGui::End();
    }

    void SubjectAddedWindow()
    {
        ImGui::Begin("Dodano przedmiot!", NULL, functional_windows_flags);
        ImGui::Text("Dodano przedmiot poprawnie");
        if (ImGui::Button("Ok"))
            subject_added_window = false;
        ImGui::End();
    }

    void SubjectRejectedWindow()
    {
        ImGui::Begin("Przedmiot juz istnieje!", NULL, functional_windows_flags);
        ImGui::Text("Przedmiot o podanej nazwie zostal juz dodany do tego semestru");
        if (ImGui::Button("Ok"))
            subject_rejected_window = false;
        ImGui::End();
    }

    static void PrintAddSubjectWindow()
    {
        disable_main_window = true;
        static char MyBuff[256] = "Nazwa przedmiotu";
        ImGui::Begin("Dodaj semestr", NULL, functional_windows_flags);
        ImGui::Text("Wpisz nazwe przedmiotu");
        ImGui::InputText(" ", MyBuff, IM_ARRAYSIZE(MyBuff));
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.23f, 0.94f, 0.66f));
        if (ImGui::Button("Dodaj"))
        {
            for (int i = 0; i < semy.size(); i++)
            {
                if (semy[i].name == currentSem)
                    if (DoesSubjectNotExist(MyBuff))
                    {
                        semy[i].subjects.push_back(MyBuff);    //dodanie przedmiotu do wybranego obecnie semestru
                        AddSubjectToSemester(i, MyBuff);
                        subject_added_window = true;
                        disable_main_window = false;
                        subject_window = false;
                    }
                    else
                    {
                        subject_rejected_window = true;
                        disable_main_window = false;
                        subject_window = false;
                    }
            }
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.85f, 0.80f));
        if (ImGui::Button("Anuluj"))
        {
            disable_main_window = false;
            subject_window = false;
        }
        ImGui::PopStyleColor();
        ImGui::End();
    }

    void PrintSemesterErrorMessage()
    {
         ImGuiWindowFlags error_flag = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar |
             ImGuiWindowFlags_NoCollapse |ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking;
         ImGui::Begin("Blad",NULL,error_flag);
         ImGui::Text("Ten semestr juz istnieje");
         ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.85f, 0.80f));
         if (ImGui::Button("Anuluj"))
             semester_error_window = false;
         ImGui::PopStyleColor();
         ImGui::End();
    }

    static void AddSemesterWindow()
    {
        disable_main_window = true;
        static int number = 0;
        ImGui::Begin("Dodaj semestr", NULL, functional_windows_flags);
        ImGui::BeginDisabled(semester_error_window);
        ImGui::Text("Podaj numer semestru");
        ImGui::InputInt("Semestr", &number);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.23f, 0.94f, 0.66f));
        if (ImGui::Button("Dodaj"))
        {
            if (IsSemNumberNotUsed(number))
            {
                std::string text = "Semestr ";
                text += std::to_string(number);
                Sem sem(text);
                semy.push_back(sem);
                SortSemsList();
                json jsonVector = json::array();
                std::string jsonString;
                for (auto& sem : semy)
                {
                    json jsonSem;
                    jsonSem["name"] = sem.name;
                    json jsonSubjects = json::array();
                    jsonSem["subjects"] = jsonSubjects;
                    jsonVector.push_back(jsonSem);
                }
                jsonString = jsonVector.dump(4);

                std::ofstream outFile("semesters.json");
                if (outFile.good())
                {
                    outFile << jsonString;
                    outFile.close();
                }
                else
                    std::cerr << "Error: failed to open file for writing\n";
                disable_main_window = false;
                semester_window = false;
            }
            else
                semester_error_window = true;
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.85f, 0.80f));
        if (ImGui::Button("Anuluj"))
        {
            disable_main_window = false;
            semester_window = false;
        }
        ImGui::PopStyleColor();
        ImGui::EndDisabled();
        ImGui::End();
    }

    void DockWindow()
    {
        // Ponizej kod do dokujacego okna w ktorym sa zadokowane moje rzeczy
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_NoResize;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
    }

    void Rendering()
    {
        DockWindow();

        // Odtad w dol moje rzeczy
        //
        // Wyswietlanie okien
        if (subject_window) PrintAddSubjectWindow();
        if (semester_window) AddSemesterWindow();
        if (semester_error_window) PrintSemesterErrorMessage();
        if (subject_added_window) SubjectAddedWindow();
        if (subject_rejected_window) SubjectRejectedWindow();
        if (delete_subject_window) DeleteSubjectWindow();
        
        // Lewe okno
        ImGuiWindowFlags window_flags_zaliczenia=0;
        window_flags_zaliczenia |= ImGuiWindowFlags_NoScrollbar;
        window_flags_zaliczenia |= ImGuiWindowFlags_MenuBar;
        window_flags_zaliczenia |= ImGuiWindowFlags_NoCollapse;
        window_flags_zaliczenia |= ImGuiWindowFlags_NoMove;

        ImGui::Begin("Semestry", nullptr, window_flags_zaliczenia);

        ImGui::BeginDisabled(disable_main_window);
        if (ImGui::BeginMenuBar())  // LISTA DO ZARZADZANIA SEMAMI
        {
            
            if (ImGui::BeginMenu("Zarzadzaj semestrami"))
            {
                ImGui::MenuItem("Dodaj semestr", NULL, &semester_window);
                ImGui::MenuItem("Usun semestr");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::BeginMenuBar())  // LISTA DO ZARZADZANIA PRZEDMIOTAMI
        {
            if (ImGui::BeginMenu("Zarzadzaj przedmiotami"))
            {
                ImGui::MenuItem("Dodaj przedmiot",NULL, &subject_window);
                ImGui::MenuItem("Usun przedmiot", NULL, &delete_subject_window);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (semy.size() > 0)    // LISTA Z WYBOREM SEMOW
        {
            if (ImGui::BeginMenuBar())
            {
                static int sem_current = 0;
                const char* preview = semy[sem_current].name.c_str();
                if (ImGui::BeginCombo("semy", preview))
                {
                    for (int n = 0; n < semy.size(); n++)
                    {
                        const bool is_selected = (sem_current == n);
                        if (ImGui::Selectable(semy[n].name.c_str(), is_selected))
                            sem_current = n;

                        // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                currentSem = preview;
                ImGui::EndMenuBar();
            }
        }
        SemesterSubjectsTable(currentSem);
        ImGui::EndDisabled();
        ImGui::End();


        //prawe okno
        ImGuiWindowFlags window_flags_semestry=0;
        window_flags_semestry |= ImGuiWindowFlags_NoScrollbar;
        window_flags_semestry |= ImGuiWindowFlags_MenuBar;
        window_flags_semestry |= ImGuiWindowFlags_NoCollapse;
        window_flags_semestry |= ImGuiWindowFlags_NoMove;
        ImGui::Begin("Informacje o przedmiocie", nullptr,window_flags_semestry);
        
        ImGui::End();


        ImGui::End();


        //na sam koniec do wyrzucenia
        ImGui::ShowDemoWindow();

    }
}
