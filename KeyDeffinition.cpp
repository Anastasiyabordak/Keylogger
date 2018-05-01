#include "stdafx.h"
#include "KeyDeffinition.h"


KeyDeffinition::KeyDeffinition(const string &key, const string &key_name)
{
	this->_key = key;
	this->_keyName = key_name;
}

string KeyDeffinition::getNameVK()
{
	return this->_keyName;
}

KeyDeffinition::~KeyDeffinition()
{
}
