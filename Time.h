#pragma once
class Time
{
private:
	int _hour;
	int _minute;
	int _second;
public:
	void setTime(int hour, int minute, int second);
	int getMinute();
	int getHour();
	int getSecond();
	~Time();
};

