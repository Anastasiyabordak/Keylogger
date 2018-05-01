#include "stdafx.h"
#include "Timer.h"
#include <thread>
#include <iostream>

void Timer::sleepRun()
{
	std::this_thread::sleep_for(this->_interval);
	if (this->_alive)
		this->_function();
}

void Timer::threadFunction()
{
	if (this->_callNumber == this->_infinite)
		while (this->_alive)
			this->sleepRun();
	else
		while (this->_repeatCounter--)
			this->sleepRun(); 
}

void Timer::setFunction(const std::function<void(void)>& fun)
{
	this->_function = fun;
}


void Timer::Start(bool asyncro)
{
	if (this->getAlive())
		return;
	this->_alive = true;
	this->_repeatCounter = this->_callNumber;
	if (asyncro)
	{
	   this->_thread = std::thread (&Timer::threadFunction, this);
	}
	else this->threadFunction();
}

void Timer::Stop()
{
	this->_alive = false;
	this->_thread.join();
}

bool Timer::getAlive() const
{
	return this->_alive;
}

void Timer::repeatCount(const long rep)
{
	if (this->_alive)
		return;
	this->_callNumber = rep;
}

long Timer::getCounter() const
{
	return this->_repeatCounter;
}

long Timer::getRepeatCount() const
{
	return this->_callNumber;
}

void Timer::setInterval(const unsigned long & i)
{
	if (this->_alive)
		return;
	this->_interval = std::chrono::milliseconds(i);
}

unsigned long Timer::getInterval() const
{
	return this->_interval.count();
}

const std::function<void(void)>& Timer::getFunction() const
{
	return this->_function;
}

Timer::~Timer()
{
}
