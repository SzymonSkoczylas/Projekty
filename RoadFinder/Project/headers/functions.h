#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <array>

/// Klasa odpowiedzialna za przechowanie informacji pobranych z jednej linii z pliku rozkładu jazdy
class FileLine 
{
private:	
	std::string m_departure;			///< zmienna przechowująca informację o miecie, z którego odjeżdzamy
	std::string m_arrival;				///< zmienna przechowująca informację o mieście docelowego
	int m_duration{};					///< zmiennna przechowująca informację o czasie jazdy z miasta, z którego odjeżdzamy do miasta docelowego 	 
	std::array<std::string,3> m_time;	///< zmienna przechowująca informację o godzinach, o których możemy odjechać z tego przystanku

public:
	/// Funkcja, która zwraca nazwę miasta (odjazdu albo docelowego) zależnie od podanej wartości parametru boolean
	/// @param boolean zmienna na podstawie której funkcja zwróci jedno z dwóch miast  
	/// @return Funkcja zwraca string zawierający nazwę miasta
	std::string GetCityName(const std::string & boolean) const 
	{
		std::string city = boolean=="arrival"? m_arrival : m_departure;
		return city;
	}

	/// Funkcja zwracająca czas jazdy z miasta odjazdu do miasta docelowego
	int GetDuration() const { return m_duration; }

	/// Funkcja zwracająca informację o liczbie godzin, o których możemy rozpocząć podróż do miasta docelowego
	int GetNumberOfHoursOfDepartures() const { return m_time.size(); }

	/// Funkcja zwracająca jedną z godzin, o której możemy rozpocząć podróż
	/// @param n zmienna na podstawie której funkcja zwróci godzinę
	int GetHour(short n) const { return ConvertHour(n); }

	/// Funkcja która na podstawie linii z pliku rozkładu jazdy zapisze wartości do zmiennych klasy
	/// @param str String, który zawiera jedną linijkę z pliku rozkładu jazdy
	void GetFileLine(const std::string& str) 
	{
		std::stringstream ss;
		ss << str;
		ss >> m_departure >> m_arrival >> m_duration >> m_time[0] >> m_time[1] >> m_time[2];
	}
private:
	/// Funkcja, która zwraca daną godzinę w postaci liczby minut w godzinie (np. 1:20 to 80)
	/// @param n zmienna na podstawie której funkcja wybiera godzinę
	int ConvertHour (const short & n) const
	{
		std::string hours;
		std::string minutes;
		if (m_time[n].size() == 5) 
		{
			hours = m_time[n].substr(0, 2);
			minutes = m_time[n].substr(3, 2);
		}
		else
		{
			hours = m_time[n].substr(0, 1);
			minutes = m_time[n].substr(2, 2);
		}
		int h = std::stoi(hours);
		int m = std::stoi(minutes);
		h *= 60;
		return h + m;
	}
};

/// Klasa zawierająca informację o nazwie miasta oraz wszystkich miast do których możemy z tego miasta dojechać
class City
{
public:
	std::string m_name;							///< nazwa miasta
	std::vector<std::string> m_canTravelTo;		///< miasta, do których możemy z tego miasta dojechać
public:

	/// Funkcja ustawiająca nazwę miasta
	/// @param str zmienna która zawiera nazwę miasta
	void SetName(const std::string & str) { m_name = str; }

	/// Funkcja, która dopisuje do vectora zawierającego informację o miastach, do których możemy dojechać kolejne z miast
	/// @param str zmienna zawierające nazwę miasta, do którego możemy dojechać
	void CanTravel(const std::string & str) { m_canTravelTo.push_back(str); }

	/// Funkcja usuwająca wartości w vectorze m_canTravelTo
	void ClearVec() { m_canTravelTo.clear(); }
};

/// Funkcja, która na podstawie vectora klas FileLine tworzy vector klas City.
/// @param timetable Gotowy kontener zawierający informację o każdej linii pliku rozkładu jazdy 
/// przechowywanej w postaci klasy
std::vector<City> FillCityVec(std::vector<FileLine> & timetable);

/// Funkcja, która szuka wszystkie możliwe drogi "z miasta A do miasta B"
/// @param goal zmienna przechowująca nazwę miasta docelowego
/// @param from zmienna przechowująca nazwę miasta odjazdu
/// @param cities kontener klas zawierający wszystkie połączenia między miastami
/// @param solution zmienna tymczasowa, która jest potrzebna do poprawnego działania algorytmu, zawiera jedną drogę (wariacja połączeń miast)
/// @param solutions kontener, który zostanie zwrócony. Zawiera wszystkie połączenia "z miasta A do miasta B", które będą korygowane w funkcji Correction()
/// 
/// Funkcja jest rekurencyjna. Algorytm szuka wszystkich miast, które mogą dojechać do danego miasta, do którego chcemy dojechać.
/// Następnie szuka wszystkich możliwych miast, z których możemy dojechać do miasta, które jest w stanie dojechać do pierwotnego miasta docelowego etc.
/// ### Element szukający pasujące miasta
/// ~~~~~~~~~~~~~~~~~~~.cpp
/// for (auto it = cities.begin(); it != cities.end(); it++)
/// {
/// 	bool condition = IfDuplicates(solution, it->m_name); //Funkcja IfDuplicates() zwraca true jeśli w vectorze, w którym zapisujemy naszą aktualną drogę
/// 	if (!condition)										 //znajduje się już miasto do którego chcemy dojechać 
/// 	{
/// 		for (auto jt = it->m_canTravelTo.begin(); jt != it->m_canTravelTo.end(); jt++)
/// 		{
/// 			if (*jt == goal)
/// 			{
/// 				solution.push_back(goal);
/// 				FindRoute(it->m_name, from, cities, solution, solutions);
/// 			}
/// 		}
/// 	}
/// }
/// ~~~~~~~~~~~~~~~~~~~
/// ### Warunek końca rekurencji
/// ~~~~~~~~~~~~~~~~~~~.cpp
///	if (goal == from)
///	{
///		solution.push_back(from);
///		solutions.push_back(solution);
///		solution.clear();
///		return;
///	}
/// ~~~~~~~~~~~~~~~~~~~
/// Funkcja zawiera w efekcie końcowym parę wyników, które nie są potrzebne dla działania programu i są korygowane w funkcji Correction()
/// @see Correction()
void FindRoute(std::string goal, const std::string& from,const std::vector<City>& cities, std::vector<std::string>& solution, std::vector<std::vector<std::string>>& solutions);

/// Funkcja korekcyjna dla funkcji FindRoute(). Jest potrzebna do optymalizacji wyników uzyskanych dzięki funkcji FindRoute. 
///	@param roads Vector przechowujący wszystki drogi(które są vectorami stringów)
/// @param goal Parametr zawierający nazwę miasta docelowego
/// Vector przechowujący wszystkie drogi aktualnie zawiera parę niechcianych wyników.
/// Wszystkie drogi, które nie kończą się miastem docelowym zostaną usunięte z vectora
/// ### Element usuwający 
/// ~~~~~~~~~~~~~~~~~~~.cpp
/// //aktualnie wszystkie drogi są w kolejności od miasta docelowego do miasta, z którego odjeżdzamy
/// for (int i=0;i<roads.size();i++)
/// {
///		std::reverse(roads[i].begin(), roads[i].end());  //odwracamy miasta i teraz są w kolejności od miasta, z którego odjeżdzamy do miasta docelowego
///		if (roads[i].back() != goal)
///			roads[i].clear();	//jeśli droga nie kończy się na mieście docelowonym usuwamy całą zawartość vectora
/// }
/// //Pod koniec funkcji elementy puste zostają usunięte z vectora dróg
/// ~~~~~~~~~~~~~~~~~~~
/// ### Element usuwający duplikaty z vectora dróg
/// ~~~~~~~~~~~~~~~~~~~.cpp
/// auto end = roads.end();
/// for (auto it = roads.begin(); it != end; it++)
///		end = std::remove(it + 1, end, *it);
/// roads.erase(end, roads.end());
/// ~~~~~~~~~~~~~~~~~~~
/// @see FindRoute()
void Correction(std::vector<std::vector<std::string>>& roads, const std::string& goal);


/// Funkcja, która na podstawie kontenera roads, znajdzie najszybszą z dróg
/// @param roads kontener zawierający wszystkie połączenia "z miasta A do miasta B"
/// @param timetable kontener zawierający informację o wszystkiech połączeniach (takie jak czas jazdy, godziny odjazdu etc.)
/// ### Algorytm sprawdzający, która z dróg jest najszybsza
/// ~~~~~~~~~~~~~~~~~~~.cpp
/// int hourOfDepartFromLastCity; // zmienna potrzebna do przechowania informacji o godzinie, o której podróżnik będzie w mieście, do którego przyjedzie
/// for (auto it = roads.begin(); it != roads.end(); it++)
///	{
///		time = 0; // zerowanie zmiennej, która służy do liczenia łącznego czasu podróży
///		temp = *it; 
///		for (size_t i = 0; i < temp.size() - 1; i++)
///		{
///			for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
///			{
///				if ((jt->GetCityName("departure") == temp[i]) && (jt->GetCityName("arrival") == temp[i + 1]))
///				{
///					time += jt->GetDuration(); // dodanie do sumy czasu podróży szacownego czasu podróży "z miasta A do miasta B" 
///					if (FirstCity)
///					{
///						hourOfDepartFromLastCity = jt->GetHour(0); 	   // jeżeli to pierwsze miasto z drogi zaczynamy od pierwszej godziny, o której możemy odjechać
///						hourOfDepartFromLastCity += jt->GetDuration(); // dodajemy to znalezionej godziny czas podróży
///						FirstCity = false;
///					}
///					else
///					{
///						for (int i = 0; i < jt->GetNumberOfHoursOfDepartures(); i++)
///						{
///							if (jt->GetHour(i) > (hourOfDepartFromLastCity+MINUMUM_TIME_FOR_CHANGE)) // jeżeli to nie pierwsze miasto z drogi szukamy następnej
///							{																		 // najwcześniejszej godziny (uwzględniając czas na przesiadkę)
///								time += jt->GetHour(i) - hourOfDepartFromLastCity;
///								hourOfDepartFromLastCity = jt->GetHour(i);
///								hourOfDepartFromLastCity += jt->GetDuration();
///								goto foundNextHour; // jeżeli następna godzina odjazdu została odnaleziona szukamy od razu godziny dla następnej pary miast
///							}
///						}
///					}
///				foundNextHour:;
///				}
///			}
///		}
///		if (time < tempTime)
///			sol = temp;
///		tempTime = time; // dodatkowa zmienna potrzebna do porównania czasów
///		FirstCity = true;
///	}
/// ~~~~~~~~~~~~~~~~~~~
std::vector<std::string> FastestRoad(const std::vector<std::vector<std::string>>& roads, const std::vector<FileLine>& timetable);

/// Funkcja, która na podstawie liczby minut podanej w postaci int wygeneruje napis zawierający informacje o tym
/// jaka to jest godzina
/// @param minutes liczba minut
std::string ConvertMinutesToHour(int& minutes);

/// Funkcja szukająca następnej godziny odjazdu "z miasta A do miasta B" względem podanej godziny
/// @param lastHour informacja o godzinie, od której chcemy znaleść kolejną godzinę odjazdu
/// @param departCity miasto, z którego odjeżdzamy
/// @param arriveCity miasto docelowe
/// @param timetable zawierający informację o wszystkich połączeniach (takie jak czas jazdy, godziny odjazdu etc.)
/// ### Algorytm szukający następnej godziny
/// ~~~~~~~~~~~~~~~~~~~.cpp
/// bool cond=false;
///	for (auto it = timetable.begin(); it != timetable.end(); it++)
///	{
///		if ((it->GetCityName("departure") == departCity) && (it->GetCityName("arrival") == arriveCity))
///		{
///			for (int i = 0; i < it->GetNumberOfHoursOfDepartures(); i++)
///			{
///				cond = true; // jeżeli funkcja nie skończy się po wykonaniu tej pętli to znaczy, że w danym dniu już nie ma odjazdów
///				if (lastHour < (it->GetHour(i) - MINUMUM_TIME_FOR_CHANGE))
///					return it->GetHour(i);
///			}
///			if (cond == true) return it->GetHour(0); // funkcja zwraca pierwszą godzinę, z której można odjechać (czyli trzeba czekać do następnego dnia)
///		}
///	}
/// ~~~~~~~~~~~~~~~~~~~
int LookForNextDepart(const int & lastHour, const std::string& departCity, const std::string& arriveCity,const std::vector<FileLine>& timetable);

/// Funkcja, która na podstawie podanej drogi liczy łączny czas jazdy (razem z czasem oczekiwania na odjazd)
/// @param road dane połączenie "z miasta A do miasta B"
/// @param timetable zawierający informację o wszystkich połączeniach (takie jak czas jazdy, godziny odjazdu etc.)
int GetTotalTime(const std::vector<std::string>& road, const std::vector<FileLine>& timetable);

#endif //FUNCTIONS_H