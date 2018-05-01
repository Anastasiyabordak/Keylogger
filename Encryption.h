//using base64 encryption
#pragma once
#include <vector>
#include <string>
using namespace std;
class Encryption
{
private :
	// Stephen Fry  quotes
	const string &_base1 = "Books are no more threatened by Kindle than stairs by elevators";
	const string &_base2 = "An original idea.That can't be too hard. The library must be full of them";
	const string &_base3 = "But you can't argue that the world isn't in an unhealthy moral state";
	const string &_baseCodes = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
public:
	string encode(const string& data);
	string encrypt(string data);
	Encryption();
	~Encryption();
};

