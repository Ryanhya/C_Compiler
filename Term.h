#pragma once
#include <string>
#include <vector>

using namespace std;


//项的数据结构
class Term {
public:
	bool is_core = false;//项目是否为核心项目
	int type;//项目类型（如S->a.bA,#），1表示移进项目，2表示归约项目（如S->A.,#），3表示待约项目
	string pleft;//产生式左部
	string pright;//产生式右部
	vector<string> right;//产生式右部
	int dot = -1;//活前缀位置，dot值加一即为下一个符号的索引
	string forward;//向前搜索符
	bool termEqual(Term t);//判断当前Term与t是否相等
};

