#pragma once
#include "Time.h"
#include "Date.h"
#include <ctime>
#include <string>
#include <sstream>
using namespace std;
/*
contains current time and date
writes applog file
*/
class CurrentTimeDate : public Time, public Date
{
private:
	string getDateString();
	string getTimeString();
public:
	void writeAppLog(const string data);	
	template <class TYPE>
	string toString(const TYPE &data);
	CurrentTimeDate();
	string getTimeDateString();
	CurrentTimeDate(int day, int month, int year, int hour, int minute, int second);
	
	~CurrentTimeDate();
};

template<class TYPE>
inline string CurrentTimeDate::toString(const TYPE & data)
{
	ostringstream stream;
	stream << data;
	return  stream.str();

}
