#pragma once
#include <string>
#include <list>
#include "SymbolList.h"
#include "WordList.h"

using namespace std;


const int N = 50;//可以存储的最大源代码行数
const string keys[] = { "main","int","char","bool","true","false","void","if","else","while","for" };//关键字集合

class LexicalAnalysis {
public:		
	WordList wList;
	SymbolList sList;//符号表
	string codeLine[N];//存储每一行源代码，在源程序的行数即为其索引+1
	bool flag = false;//是否出现了多行注释的前缀/*
	void readSourceFile(string path);//读取源代码文件
	bool isDigit(char c);//判断字符是否为数字
	bool isLetter(char c);//判断字符是否为字母
	bool isKey(string word);//判断单词是否为关键字
	bool idExisted(string name);//标识符是否已经存在
	bool isSeparated(char c);//判断是否为分界符
	bool isOperator(char c);//判断是否为单个字符的运算符即+-*/><=!
	void lexAnalysisCodeLine(string code, int line);//进行词法分析,每次分析一行源代码，填写单词列表和符号表


};

