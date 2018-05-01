#pragma once
#include "KeyDeffinition.h"
#include <Windows.h>
class KeyMap
{
private:
	static const map<int, KeyDeffinition> _keys;
public:
	string getName(const KBDLLHOOKSTRUCT *key);
	KeyMap();
	~KeyMap();
};

