#include "stdafx.h"
#include "Encryption.h"


string Encryption::encode(const string & data)
{
	string temp;
	int val = 0;
	int bits = -6;
	const unsigned int base = 0x3F;
	for (const auto &c : data)
	{
		val = (val << 8) + c;
		bits += 8;
		while (bits >= 0)
		{
			temp.push_back(this->_baseCodes[(val >> bits) & base]);
			bits -= 6;
		}
	}
	if (bits >= -6)
		temp.push_back(this->_baseCodes[((val << 8) >> (bits + 8)) & base]);
	while (temp.size() % 4)
		temp.push_back('-');
	return temp;   
}

string Encryption::encrypt(string data)
{
	data = this->_base1 + data + this->_base2 + this->_base3 ;
	data = this->encode(data); 
	data.insert(7, this->_base3);
	data += this->_base1;
	data = this->encode(data);
	data = this->_base1 + this->_base2 + this->_base3;
	data.insert(1, "L");
	data.insert(7, "M");
	return data; 
}

Encryption::Encryption()
{
	
	
}


Encryption::~Encryption()
{
}
