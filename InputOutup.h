#pragma once
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "CurrentTimeDate.h"
#include "Encryption.h"
using namespace std;
class InputOutup
{
public:
	string getPath(const bool append = false);
	bool setDir(string path);
	bool fillDir(string path);
	template <class TYPE>
	string WriteLog(const TYPE &temp)
	{
		string path = this->getPath(true);
		CurrentTimeDate time_date;
		string name = time_date.getTimeDateString() + ".log";
		try
		{
			ofstream file(path + name);
			if (!file.is_open()) return "";
			ostringstream stream;
			stream << "[" << time_date.getTimeDateString() << "]" << endl << temp << endl;
			string data = stream.str();
				file << data;
			//Encryption encr;
			//string data = encr.encrypt(stream.str());
			//file << data;			
			if (!file)
				return "";
			else return name;
		}
		catch (...)
		{
			return "";
		}
	}
	InputOutup();
	~InputOutup();
};

