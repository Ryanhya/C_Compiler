#pragma once
#include <string>
#include <vector>


using namespace std;


class Production {
public:
	string pleft;//产生式左部
	string pright;//产生式右部
	vector<string> right;//产生式右部
};
