#include "functions.h"

#define MINUMUM_TIME_FOR_CHANGE 5

std::vector<City> FillCityVec(std::vector<FileLine> & timetable)
{
	std::vector<City> vec;
	std::string temp;
	bool cond = 0;
	std::unique_ptr<City> city = std::make_unique<City>();
	for (auto it = timetable.begin(); it != timetable.end(); it++)
	{
		temp = it->GetCityName("departure");
		for (auto jt = vec.begin(); jt != vec.end(); jt++)
		{
			if (jt->m_name == temp)
				cond = 1;
		}
		if (cond == 0)
		{
			city->SetName(temp);
			for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
			{
				if (jt->GetCityName("departure") == temp)
					city->CanTravel(jt->GetCityName("arrival"));
			}
			vec.push_back(*city);
			city->ClearVec();
		}
		cond = 0;
	}
	for (auto it = timetable.begin(); it != timetable.end(); it++)
	{
		cond = 1;
		temp = it->GetCityName("arrival");
		for (auto jt = vec.begin(); jt != vec.end(); jt++)
		{
			if (jt->m_name == temp)
				cond = 0;
		}
		if (cond)
		{
			city->SetName(temp);
			vec.push_back(*city);
		}
	}
	return vec;
}

/// Funkcja sprawdzająca czy w danej drodze nie wystąpiło już dane miasto
/// @param vec dana droga
/// @param city miasto, które sprawdzamy, czy już nie wystąpiło
/// @return wartość true jeśli miasto występuje, false jeśli przeciwnie
bool IfDuplicates(const std::vector<std::string>& vec, const std::string& city)
{
	std::string str;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		str = *it;
		if (str == city) return true;
	}
	return false;
}

void Correction(std::vector<std::vector<std::string>> & roads, const std::string & goal)
{
	for (int i=0;i<roads.size();i++)
	{
		std::reverse(roads[i].begin(), roads[i].end());
		if (roads[i].back() != goal)
			roads[i].clear();
	}

	auto end = roads.end();
	for (auto it = roads.begin(); it != end; it++)
		end = std::remove(it + 1, end, *it);
	roads.erase(end, roads.end());

	auto it = roads.begin();
	for (int i = 0; i < roads.size(); i++)
	{
		if (roads[i].empty() == true)
		{
			it += i;
			roads.erase(it);
		}
	}
}
void FindRoute(std::string goal, const std::string & from,const std::vector<City>& cities, std::vector<std::string>& solution, std::vector<std::vector<std::string>>& solutions)
{
	if (goal == from)
	{
		solution.push_back(from);
		if (!solution.empty()) solutions.push_back(solution);
		solution.clear();
		return;
	}
	for (auto it = cities.begin(); it != cities.end(); it++)
	{
		bool condition = IfDuplicates(solution, it->m_name);
		if (!condition)
		{
			for (auto jt = it->m_canTravelTo.begin(); jt != it->m_canTravelTo.end(); jt++)
			{
				if (*jt == goal)
				{
					condition = IfDuplicates(solution, *jt);
					if (!condition)
					{
						solution.push_back(goal);
						FindRoute(it->m_name, from, cities, solution, solutions);
					}
				}
			}
		}
	}
	solution.clear();
}

std::vector<std::string> FastestRoad(const std::vector<std::vector<std::string>>& roads,const std::vector<FileLine> & timetable)
{
	std::vector<std::string> temp;
	std::vector<std::string> sol;
	std::vector<FileLine>::iterator ptr;
	bool FirstCity = true;
	int hourOfDepartFromLastCity;
	int time{};
	int tempTime = 1441;
	for (auto it = roads.begin(); it != roads.end(); it++)
	{
		time = 0;
		temp = *it;
		for (size_t i = 0; i < temp.size() - 1; i++)
		{
			for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
			{
				if ((jt->GetCityName("departure") == temp[i]) && (jt->GetCityName("arrival") == temp[i + 1]))
				{
					time += jt->GetDuration();
					if (FirstCity)
					{
						hourOfDepartFromLastCity = jt->GetHour(0);
						hourOfDepartFromLastCity += jt->GetDuration();
						FirstCity = false;
					}
					else
					{
						for (int i = 0; i < jt->GetNumberOfHoursOfDepartures(); i++)
						{
							if (jt->GetHour(i) > (hourOfDepartFromLastCity+MINUMUM_TIME_FOR_CHANGE))
							{
								time += jt->GetHour(i) - hourOfDepartFromLastCity;
								hourOfDepartFromLastCity = jt->GetHour(i);
								hourOfDepartFromLastCity += jt->GetDuration();
								goto foundNextHour;
							}
						}
					}
				}foundNextHour:;
			}
		}
		if (time < tempTime)
			sol = temp;
		tempTime = time;
		FirstCity = true;
	}
	return sol;
}

std::string ConvertMinutesToHour(int & minutes)
{
	std::string hours;
	int temp = minutes % 60;
	minutes -= temp;
	minutes /= 60;
	hours = std::to_string(minutes);
	hours += ':';
	if (temp < 10) hours+='0';
	hours += std::to_string(temp);
	return hours;
}

int LookForNextDepart(const int & lastHour, const std::string & departCity, const std::string & arriveCity,const std::vector<FileLine> & timetable)
{
	bool cond=false;
	for (auto it = timetable.begin(); it != timetable.end(); it++)
	{
		if ((it->GetCityName("departure") == departCity) && (it->GetCityName("arrival") == arriveCity))
		{
			for (int i = 0; i < it->GetNumberOfHoursOfDepartures(); i++)
			{
				cond = true;
				if (lastHour < (it->GetHour(i) - MINUMUM_TIME_FOR_CHANGE))
					return it->GetHour(i);
			}
			if (cond == true) return it->GetHour(0);
		}
	}
}

int GetTotalTime(const std::vector<std::string>& road, const std::vector<FileLine>& timetable)
{
	int departHour{};
	int arriveHour{};
	int changeTimeMinutes{};

	int totalTime{};

	for (auto it = road.begin(); it != road.end() - 1; it++)
	{
		if (it != road.end() - 2)
		{
			for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
			{
				if ((jt->GetCityName("departure") == *it) && (jt->GetCityName("arrival") == *(it + 1)))
				{
					if (it == road.begin())
						departHour = jt->GetHour(0);
					else
						departHour = LookForNextDepart(arriveHour, *it, *(it + 1), timetable);
					arriveHour = departHour + jt->GetDuration();
					changeTimeMinutes = LookForNextDepart(arriveHour, *(it + 1), *(it + 2), timetable) - arriveHour;
					if (changeTimeMinutes < 0)
					{
						auto temp = LookForNextDepart(arriveHour, *(it + 1), *(it + 2), timetable);
						int minutesInDay = 1440;
						changeTimeMinutes = minutesInDay - arriveHour + temp;
					}
					totalTime += jt->GetDuration() + changeTimeMinutes;


					goto foundCity;
				}
			}foundCity:;
		}
		else if (it == road.end() - 2)
		{
			for (auto jt = timetable.begin(); jt != timetable.end(); jt++)
			{
				if ((jt->GetCityName("departure") == *it) && (jt->GetCityName("arrival") == *(it + 1)))
				{
					totalTime += jt->GetDuration();
					return totalTime;
				}
			}
		}
	}
}