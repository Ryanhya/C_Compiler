#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <stack>
#include "SymbolList.h"
#include "Code.h"
#include "Production.h"
#include "Attribute.h"

using namespace std;

const int symbol_num = 50;//符号表中符号的最大数量
const int code_num = 100;

class SemanticAnalysis {
public:
	stack<Attribute> attributeStack;//属性栈
	Code codes[code_num];//存放中间代码
	string midresult[code_num];//存放中间结果
	int nextquad = 0;//下一条要生成的中间代码索引
	int mid_index = 0;//下一个要产生的临时变量的索引
	
	list<SymbolItem>::iterator toSymbolList[symbol_num];//i与符号表迭代器的映射


	//list<SymbolItem>::iterator lookup(list<SymbolItem> slist, string name);
	void getSymbolListIndex(list<SymbolItem> slist);//建立符号表的索引
	//void updateSymbolList(list<SymbolItem>& slist);
	int entry(string name);//获取name在符号表slist的索引
	int getAddr(string name);//获取标识符name分配的地址
	void addAddr(int index, int address);//填写符号表指定索引的地址
	void addType(int index, int type);//填写符号表指定索引的类型信息
	void Gen(string op, int arg1, int arg2, int result);//生成指定的四元式并填写四元式表
	int newtemp();//获取在中间结果内的索引
	vector<int> makelist(int quad);
	vector<int> merge(vector<int> v1, vector<int> v2);//合并
	void backpatch(vector<int> p, int quad);//地址回填

	

	void translate(Production p);//根据产生式p的翻译方案进行语法制导翻译

};