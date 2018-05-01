#include "stdafx.h"
#include "Date.h"

void Date::setDate(int day, int month, int year)
{
	this->_day = day;
	this->_month = month;
	this->_year = year;
}

int Date::getDay()
{
	return this->_day;
}

int Date::getMonth()
{
	return this->_month;
}

int Date::getYear()
{
	return this->_year;
}

Date::~Date()
{
}
