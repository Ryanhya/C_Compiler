#pragma once
#include <string>
#include <list>
#include "SymbolList.h"
#include "WordList.h"

using namespace std;


const int N = 50;//���Դ洢�����Դ��������
const string keys[] = { "main","int","char","bool","true","false","void","if","else","while","for" };//�ؼ��ּ���

class LexicalAnalysis {
public:		
	WordList wList;
	SymbolList sList;//���ű�
	string codeLine[N];//�洢ÿһ��Դ���룬��Դ�����������Ϊ������+1
	bool flag = false;//�Ƿ�����˶���ע�͵�ǰ׺/*
	void readSourceFile(string path);//��ȡԴ�����ļ�
	bool isDigit(char c);//�ж��ַ��Ƿ�Ϊ����
	bool isLetter(char c);//�ж��ַ��Ƿ�Ϊ��ĸ
	bool isKey(string word);//�жϵ����Ƿ�Ϊ�ؼ���
	bool idExisted(string name);//��ʶ���Ƿ��Ѿ�����
	bool isSeparated(char c);//�ж��Ƿ�Ϊ�ֽ��
	bool isOperator(char c);//�ж��Ƿ�Ϊ�����ַ����������+-*/><=!
	void lexAnalysisCodeLine(string code, int line);//���дʷ�����,ÿ�η���һ��Դ���룬��д�����б�ͷ��ű�


};

