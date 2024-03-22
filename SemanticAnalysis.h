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

const int symbol_num = 50;//���ű��з��ŵ��������
const int code_num = 100;

class SemanticAnalysis {
public:
	stack<Attribute> attributeStack;//����ջ
	Code codes[code_num];//����м����
	string midresult[code_num];//����м���
	int nextquad = 0;//��һ��Ҫ���ɵ��м��������
	int mid_index = 0;//��һ��Ҫ��������ʱ����������
	
	list<SymbolItem>::iterator toSymbolList[symbol_num];//i����ű��������ӳ��


	//list<SymbolItem>::iterator lookup(list<SymbolItem> slist, string name);
	void getSymbolListIndex(list<SymbolItem> slist);//�������ű������
	//void updateSymbolList(list<SymbolItem>& slist);
	int entry(string name);//��ȡname�ڷ��ű�slist������
	int getAddr(string name);//��ȡ��ʶ��name����ĵ�ַ
	void addAddr(int index, int address);//��д���ű�ָ�������ĵ�ַ
	void addType(int index, int type);//��д���ű�ָ��������������Ϣ
	void Gen(string op, int arg1, int arg2, int result);//����ָ������Ԫʽ����д��Ԫʽ��
	int newtemp();//��ȡ���м����ڵ�����
	vector<int> makelist(int quad);
	vector<int> merge(vector<int> v1, vector<int> v2);//�ϲ�
	void backpatch(vector<int> p, int quad);//��ַ����

	

	void translate(Production p);//���ݲ���ʽp�ķ��뷽�������﷨�Ƶ�����

};