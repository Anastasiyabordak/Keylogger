#pragma once

class Date
{
private:
	int _day;
	int _month;
	int _year;
public:
	
	void setDate(int day, int month, int year);
	int getDay();
	int getMonth();
	int getYear();
	~Date();
};

