#include "stdafx.h"
#include "CurrentTimeDate.h"
#include <fstream>
#include <iostream>
using namespace std;
/*
set current time and date by default, using Time::setTime and Date::setDate
*/
CurrentTimeDate::CurrentTimeDate()
{
	time_t time_struct;
	time(&time_struct);
	struct tm *data = localtime(&time_struct);
	this->setTime(data->tm_hour, data->tm_min, data->tm_sec);
	this->setDate(data->tm_mday, data->tm_mon + 1, 1900 + data->tm_year);
}

CurrentTimeDate::CurrentTimeDate(int day, int month, int year, int hour, int minute, int second)
{
	this->setDate(day, month, year);
	this->setTime(hour, minute, second);
}
/*
writing applog file
!!!WARNING!!!
this file's creating NOT in \\Appdata\\microsoft\\...\\CLR,
but where .exe situates
*/
void CurrentTimeDate::writeAppLog(const string data)
{
	ofstream file("AppLog.txt", ios::app);
	file << "[" << this->getTimeDateString() << "]" << "\n" << data << endl << "\n";
	file.close();
}
/*
get data and create string from it
*/
string CurrentTimeDate::getDateString()
{
	return string(this->getDay() < 10 ? "0" : "") + this->toString(this->getDay()) + string(this->getMonth() < 10 ? ".0" : ".") + this->toString(this->getMonth()) + "." + this->toString(this->getYear());
}
/*
get time and create string from it
*/
string CurrentTimeDate::getTimeString()
{
	return string(this->getHour() < 10 ? "0" : "") + this->toString(this->getHour()) + "." + string(this->getMinute() < 10 ? "0" : "") + this->toString(this->getMinute()) + "." + string(this->getSecond() < 10 ? "." : "") + this->toString(this->getSecond());
}
/*
create string with current date and time
*/
string CurrentTimeDate::getTimeDateString()
{
	return this->getDateString() + "__" + this->getTimeString();
}

CurrentTimeDate::~CurrentTimeDate()
{

}
