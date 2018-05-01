#include "stdafx.h"
#include "Time.h"


void Time::setTime(int hour, int minute, int second)
{
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;
}

int Time::getMinute()
{
	return this->_minute;
}

int Time::getHour()
{
	return this->_hour;
}

int Time::getSecond()
{
	return this->_second;
}


Time::~Time()
{
}
