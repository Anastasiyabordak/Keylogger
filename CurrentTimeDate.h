#pragma once
#include "Time.h"
#include "Date.h"
#include <ctime>
#include <string>
#include <sstream>
using namespace std;

class CurrentTimeDate : public Time, public Date
{
//private:
	
public:
	template <class TYPE>
	string toString(const TYPE &data);
	CurrentTimeDate();
	CurrentTimeDate(int day, int month, int year, int hour, int minute, int second);
	void writeAppLog(const string data);
	string getDateString();
	string getTimeString();
	string getTimeDateString();
	~CurrentTimeDate();
};

template<class TYPE>
inline string CurrentTimeDate::toString(const TYPE & data)
{
	ostringstream stream;
	stream << data;
	return  stream.str();

}
