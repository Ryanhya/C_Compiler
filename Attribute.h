#pragma once
#include <string>
#include <vector>
#include <list>
#include "SymbolItem.h"

using namespace std;

class Attribute {
public:
	int num_val;//常数值
	char character_val;//字符值
	bool logic_val;//布尔值
	int inh;//数据类型值
	int type;//数据类型值
	string name;//标识符名称
	list<SymbolItem>::iterator symbol_place;//在符号表的索引
	int midresult_palce;//在中间结果表中的索引
	vector<int> truelist;
	vector<int> falselist;
	vector<int> nextlist;
	vector<int> idlist;
	string op;//操作符
	int quad;//下一个四元式的索引
	bool is_Vn;//是否还有else分支
	bool exist;//for循环中初始化语句的变量类型是否声明
};
