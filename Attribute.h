#pragma once
#include <string>
#include <vector>
#include <list>
#include "SymbolItem.h"

using namespace std;

class Attribute {
public:
	int num_val;//����ֵ
	char character_val;//�ַ�ֵ
	bool logic_val;//����ֵ
	int inh;//��������ֵ
	int type;//��������ֵ
	string name;//��ʶ������
	list<SymbolItem>::iterator symbol_place;//�ڷ��ű������
	int midresult_palce;//���м������е�����
	vector<int> truelist;
	vector<int> falselist;
	vector<int> nextlist;
	vector<int> idlist;
	string op;//������
	int quad;//��һ����Ԫʽ������
	bool is_Vn;//�Ƿ���else��֧
	bool exist;//forѭ���г�ʼ�����ı��������Ƿ�����
};
