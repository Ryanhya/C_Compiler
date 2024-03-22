#pragma once
#include <string>
#include <list>
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <queue>
#include "SymbolList.h"
#include "WordList.h"
#include "Production.h"
#include "Term.h"
#include "Reflection.h"
#include "SemanticAnalysis.h"
#include "Attribute.h"

using namespace std;

const int vt_num = 38;
const int vn_num = 26;

class SyntaxAnalysis {
public:
	string start_symbol = "S\'";//文法的开始符号
	//给定文法的终结符号集
	string Vt[vt_num] = {
		"id" , "num" , "character" , "#" ,
		"$" , "(" , ")" , ";" , "," , "{" , "}" , 
		"+" , "-" , "*" , "/" , "++" , "--" ,
		"=" , ">" , "<" , ">=" , "<=" , "==" , "!=" , "&&" , "||" , "!" ,
		"void" , "int" , "char" , "bool" , "main" , "true" , "false" , "if" , "else" , "while" , "for" };
	vector<Production> grammar;//文法存储结构,每个string对应文法的一条产生式
	set<string> Vn;//非终结符号集合
	string myVn[vn_num];//非终结符号集

	int num = 0;//状态的数量
	unordered_map<int, vector<Term>> LR1_collection;//LR(1)项目规范簇存储数据结构
	vector<Reflection> reach;//映射关系的集合

	string ACTION[300][vt_num];//LR（1）分析表存储结构
	int GOTO[300][vn_num];//LR(1)分析表存储结构，为0则说明出错
	unordered_map<string, set<string>> allFirst;//First集存储结构
	unordered_map<string, set<string>> allFollow;//Follow集存储结构
	unordered_map<int, bool> flags;//一个状态是否完成派生，当flags内全为true时，说明项目规范簇构造完成


	stack<int> statusStack;//状态栈
	stack<string> symbolStack;//符号栈
	
	SemanticAnalysis mySemantic;


	set<int> emptyIndex();//返回产生式右部为空#的产生式在grammar中的索引
	bool isVt(string str);//判断str是否为终结符号



	//获取文法并划分产生式
	void readGrammarFile(string path);//读文法文件
	string getVt(string production);//获取一条产生式中的终结符号
	vector<string> splitRight(string right);//将产生式的右部划分成非终结符号和终结符号的组合

	//计算First集
	set<string> getSymbolFirst(string symbol);//计算单个符号symbol的First集
	void getFirst();//计算产生式的First集
	void writeFirstFile(string path);//输出中间结果First集

	//计算Follow集
	set<string> getSymbolFollow(string symbol);//计算非终结符号symbol的Follow集
	void getFollow();//计算产生式的Follow集
	void writeFollowFile(string path);//输出中间结果Follow集

	//自动化构造LR1项目规范簇
	void initialTermCollection(int no, vector<Term> v);//初始化项目集为其核心项目v，name：Term1、Term2...
	vector<Term> closure(Term term);//计算待归项目term的闭包
	bool isTermExist(Term t, vector<Term> v);//判断项目t是否在v中已存在
	int setTermType(Term t);//设置t的类型信息，默认值为1即移进项目，2为归约项目，3为待约项目
	void extendTerm(int no);//扩展状态no的项目，根据其核心项目求派生项目

	bool isCoreTermExist(vector<Term> v);//判断核心项目是否已经存在
	int pre;//当判断出核心项目存在时，得到该状态的标号
	bool isFlagsAllT();//判断flags内是否全为true
	void getNewCollection(int no);//对未完成派生的项目no派生新的项目集
	void buildLR1Collections(Term start);//根据开始符号产生式的LR项目，构造LR(1)项目规范簇
	void writeLR1ResultFile(string path);//将构造的LR1项目规范簇结果写入文件


	int vt2int(string s);//终结符号s转换为int的映射函数
	int vn2int(string s);//非终结符号s转换为int的映射函数，非终结符号均为大写字母
	int getProductionIndex(Term t);//获取归约项目t对应的产生式的索引
	void buildLR1Table();//参数待定，构造LR分析表
	void writeLR1TableFile(string path);//将构造的LR1分析表输出

	string tranSymbol(Word w);//将词法分析器得到的单词转化为文法中的终结符号
	char analyse(int status, string symbol, Word now_word);//一次分析
	void LRAnalyse(list<Word> words);//进行LR语法分析

	vector<string> msplit(string st, char sign);//将字符串按sign分割，返回一个容器
	void readLR1Table(string path);//读取存有LR1分析表的文件
	void showStack();//显示栈中内容

	void error(Word w);//出错处理


	


};

