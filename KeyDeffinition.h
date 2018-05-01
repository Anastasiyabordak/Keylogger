#pragma once
#include <map>
#include <string>

using namespace std;

class KeyDeffinition
{
private:
	string _key;
	string _keyName;
public:
	KeyDeffinition(const string &key = "",const string &key_name = "");
	string getNameVK();
	~KeyDeffinition();
};

