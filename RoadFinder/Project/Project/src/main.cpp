#include "functions.h" 

int main()
{
	std::ifstream inFileTimetable("..\\data\\timetable.txt"); ///< Plik rozkładu jazdy
	std::ifstream inFileTravel("..\\data\\travel.txt");		  ///< Plik z drogami, które chcemy znaleźć
	std::ofstream toSolutionFile("..\\data\\solution.txt");	  ///< Plik wyjściowy
	std::string line;
	FileLine fline;
	std::vector<FileLine> timetable;						  ///< Vector ze zinterpretowanymi liniami pliku rozkładu jazdy
	std::vector<City> cities;								  ///< Vector wszystkich miast i ich połączeń
	std::vector<City> tempCities;

	while (std::getline(inFileTimetable, line))
	{
		fline.GetFileLine(line);
		timetable.push_back(fline);
	}
	

	struct { std::string a; std::string b; } traveler;
	auto& [departCity, arriveCity] = traveler;

	std::stringstream ss;
	std::vector<std::string> temp;
	std::vector<std::string> fastest;
	std::vector<std::vector<std::string>> roads;
	int time{};
	int totalTime{};
	int departHour{};
	int arriveHour{};
	int changeTimeMinutes{};
	std::string changeTime{};

	while (std::getline(inFileTravel, line))
	{
		ss << line;
		ss >> departCity >> arriveCity;
		cities = FillCityVec(timetable);
		FindRoute(arriveCity, departCity, cities, temp, roads); ///< szukanie wszystkich dróg
		Correction(roads, arriveCity); ///< korekta wyników funkcji FindRoute
		if (!roads.empty())
		{
			fastest = FastestRoad(roads, timetable); ///< najszybsza trasa
			totalTime = GetTotalTime(fastest, timetable); ///< łączny czas podróży
			

			toSolutionFile << departCity << " --> " << arriveCity << " (czas: " <<
				ConvertMinutesToHour(totalTime) << ", przesiadki: " << fastest.size() - 2 << ")\n" <<
				"-----------------------------------------------------\n";

				for (auto it = fastest.begin(); it != fastest.end() - 1; it++)
				{
					if (fastest.size() != 2)
					{
						if (it != fastest.end() - 2)
						{
							for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
							{
								if ((jt->GetCityName("departure") == *it) && (jt->GetCityName("arrival") == *(it + 1)))
								{
									if (it == fastest.begin())
										departHour = jt->GetHour(0);
									else
										departHour = LookForNextDepart(arriveHour, *it, *(it + 1), timetable);
									arriveHour = departHour + jt->GetDuration();
									changeTimeMinutes = LookForNextDepart(arriveHour, *(it + 1), *(it + 2), timetable) - arriveHour;

									auto temp1 = arriveHour;
									auto temp2 = departHour;
									auto temp3 = changeTimeMinutes;
									if (temp3 < 0) temp3 *= -1;
									changeTime = ConvertMinutesToHour(temp3);

									toSolutionFile << jt->GetCityName("departure") << " (" << ConvertMinutesToHour(temp2) <<
										") --> " << jt->GetCityName("arrival") << " (" << ConvertMinutesToHour(temp1) <<
										"), przesiadka za " << changeTime << '\n';
									goto foundCity;
								}
							}foundCity:;
						}
						else if (it == fastest.end() - 2)
						{
							for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
							{
								if ((jt->GetCityName("departure") == *it) && (jt->GetCityName("arrival") == *(it + 1)))
								{
									departHour = changeTimeMinutes + arriveHour;
									arriveHour = departHour + jt->GetDuration();

									toSolutionFile << jt->GetCityName("departure") << " (" << ConvertMinutesToHour(departHour) <<
										") --> " << jt->GetCityName("arrival") << " (" << ConvertMinutesToHour(arriveHour) << ")\n\n";
									goto loopEnd;
								}
							}loopEnd:;
						}
					}
					else
					{
						for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
						{
							if ((jt->GetCityName("departure") == *it) && (jt->GetCityName("arrival") == *(it + 1)))
							{
								departHour = jt->GetHour(0);
								arriveHour = departHour + jt->GetDuration();

								toSolutionFile << jt->GetCityName("departure") << " (" << ConvertMinutesToHour(departHour) <<
									") --> " << jt->GetCityName("arrival") << " (" << ConvertMinutesToHour(arriveHour) << ")\n\n";
							}
						}
					}
				}
			roads.clear();
		}
		else
			toSolutionFile << departCity << " --> " << arriveCity << " - DROGA NIE ISTNIEJE\n\n";
		ss.clear();
	}
}