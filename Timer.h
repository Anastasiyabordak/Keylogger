#pragma once
#include <thread>
#include <chrono>

class Timer
{
private:
	std::thread _thread;
	bool _alive;
	long _callNumber = -1L;
	long _repeatCounter = -1L;
	static const long _infinite = -1L;
	std::chrono::microseconds _interval = std::chrono::milliseconds(0);
	std::function<void(void)> _function = nullptr;
	void sleepRun();
	
public:
	void threadFunction();
	Timer() {};
	Timer(const std::function<void(void)> &fun) : _function(fun) {}
	Timer(const std::function <void(void)> &fun, const unsigned long &i, const long repeat = _infinite) : _function(fun),
		_interval(std::chrono::milliseconds(i)),
		_callNumber(repeat){}
	void Start(bool asyncro = true);
	void Stop();
	
	void setFunction(const std::function<void(void)> &fun);
	bool getAlive() const;
	void repeatCount(const long rep);
	long getCounter() const;
	long getRepeatCount()const;
	void setInterval(const unsigned long &i);
	unsigned long getInterval()const;
	const std::function <void(void)> &getFunction()const;
	~Timer();
};

