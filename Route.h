#pragma once
#include <iostream>
#include <iomanip>
using namespace std;

class Route
{
private:
	char from[80];
	char to[80];
	char depart_day[4];        // The departure weekday
	char arrival_day[4];       // The arrival weekday
	int depart_time[2];       // The departure time in 24-hour format
	int arrival_time[2];      // The arrival time in 24-hour format
public:

	void setstations(string fromplace, string toplace)
	{
		strcpy(from, fromplace.c_str());
		strcpy(to, toplace.c_str());
	}

	void setday(std::string departure, std::string arrival)
	{
		for (int i = 0; i < 3; i++)
		{
			depart_day[i] = departure[i];
			arrival_day[i] = arrival[i];
		}
		depart_day[3] = '\0';
		arrival_day[3] = '\0';
	}

	void settime(int departure[2], int arrival[2])
	{
		depart_time[0] = departure[0];
		depart_time[1] = departure[1];
		arrival_time[0] = arrival[0];
		arrival_time[1] = arrival[1];
	}

	void getdata(stringstream& info);	  // Get information about object
	string showdata();  // Return string that contains the information about object

	bool isEqualto(Route newobj)
	// Return true if from and to are equal
	{
		return !(strcmp(from, newobj.from) || strcmp(to, newobj.to));
	}
};

void Route::getdata(stringstream& info)
// Get information about object from console
{
	info >> from >> to >>
		depart_day >> depart_time[0] >> depart_time[1] >> 
		arrival_day >> arrival_time[0] >> arrival_time[1];
	depart_day[3] = '\0';
	arrival_day[3] = '\0';
}

string Route::showdata()
// Return string that contains the information about object
{
	ostringstream info;  // string buffer to convert numbers to string

	// put float into string buffer
	info << "From: " << from << endl;
	info << "To: " << to << endl;
	if (depart_day == arrival_day)
	{
		info << "The weekday: " << arrival_day;
		info << "The departure time: " << depart_time[0] << ':';
		if (depart_time[1] < 10)
			info << 0;
		info << depart_time[1] << endl;
	}
	else
	{
		info << "The departure weekday: " << depart_day << endl;
		info << "The departure time: " << depart_time[0] << ':';
		if (depart_time[1] < 10)
			info << 0;
		info << depart_time[1] << endl;
		info << "The arrival weekday: " << arrival_day << endl;
	}
	info << "The arrival time: " << arrival_time[0] << ':';
	if (arrival_time[1] < 10)
		info << 0;
	info << arrival_time[1] << endl;

	return info.str();
}
