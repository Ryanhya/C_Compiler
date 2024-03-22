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
	string start_symbol = "S\'";//�ķ��Ŀ�ʼ����
	//�����ķ����ս���ż�
	string Vt[vt_num] = {
		"id" , "num" , "character" , "#" ,
		"$" , "(" , ")" , ";" , "," , "{" , "}" , 
		"+" , "-" , "*" , "/" , "++" , "--" ,
		"=" , ">" , "<" , ">=" , "<=" , "==" , "!=" , "&&" , "||" , "!" ,
		"void" , "int" , "char" , "bool" , "main" , "true" , "false" , "if" , "else" , "while" , "for" };
	vector<Production> grammar;//�ķ��洢�ṹ,ÿ��string��Ӧ�ķ���һ������ʽ
	set<string> Vn;//���ս���ż���
	string myVn[vn_num];//���ս���ż�

	int num = 0;//״̬������
	unordered_map<int, vector<Term>> LR1_collection;//LR(1)��Ŀ�淶�ش洢���ݽṹ
	vector<Reflection> reach;//ӳ���ϵ�ļ���

	string ACTION[300][vt_num];//LR��1��������洢�ṹ
	int GOTO[300][vn_num];//LR(1)������洢�ṹ��Ϊ0��˵������
	unordered_map<string, set<string>> allFirst;//First���洢�ṹ
	unordered_map<string, set<string>> allFollow;//Follow���洢�ṹ
	unordered_map<int, bool> flags;//һ��״̬�Ƿ������������flags��ȫΪtrueʱ��˵����Ŀ�淶�ع������


	stack<int> statusStack;//״̬ջ
	stack<string> symbolStack;//����ջ
	
	SemanticAnalysis mySemantic;


	set<int> emptyIndex();//���ز���ʽ�Ҳ�Ϊ��#�Ĳ���ʽ��grammar�е�����
	bool isVt(string str);//�ж�str�Ƿ�Ϊ�ս����



	//��ȡ�ķ������ֲ���ʽ
	void readGrammarFile(string path);//���ķ��ļ�
	string getVt(string production);//��ȡһ������ʽ�е��ս����
	vector<string> splitRight(string right);//������ʽ���Ҳ����ֳɷ��ս���ź��ս���ŵ����

	//����First��
	set<string> getSymbolFirst(string symbol);//���㵥������symbol��First��
	void getFirst();//�������ʽ��First��
	void writeFirstFile(string path);//����м���First��

	//����Follow��
	set<string> getSymbolFollow(string symbol);//������ս����symbol��Follow��
	void getFollow();//�������ʽ��Follow��
	void writeFollowFile(string path);//����м���Follow��

	//�Զ�������LR1��Ŀ�淶��
	void initialTermCollection(int no, vector<Term> v);//��ʼ����Ŀ��Ϊ�������Ŀv��name��Term1��Term2...
	vector<Term> closure(Term term);//���������Ŀterm�ıհ�
	bool isTermExist(Term t, vector<Term> v);//�ж���Ŀt�Ƿ���v���Ѵ���
	int setTermType(Term t);//����t��������Ϣ��Ĭ��ֵΪ1���ƽ���Ŀ��2Ϊ��Լ��Ŀ��3Ϊ��Լ��Ŀ
	void extendTerm(int no);//��չ״̬no����Ŀ�������������Ŀ��������Ŀ

	bool isCoreTermExist(vector<Term> v);//�жϺ�����Ŀ�Ƿ��Ѿ�����
	int pre;//���жϳ�������Ŀ����ʱ���õ���״̬�ı��
	bool isFlagsAllT();//�ж�flags���Ƿ�ȫΪtrue
	void getNewCollection(int no);//��δ�����������Ŀno�����µ���Ŀ��
	void buildLR1Collections(Term start);//���ݿ�ʼ���Ų���ʽ��LR��Ŀ������LR(1)��Ŀ�淶��
	void writeLR1ResultFile(string path);//�������LR1��Ŀ�淶�ؽ��д���ļ�


	int vt2int(string s);//�ս����sת��Ϊint��ӳ�亯��
	int vn2int(string s);//���ս����sת��Ϊint��ӳ�亯�������ս���ž�Ϊ��д��ĸ
	int getProductionIndex(Term t);//��ȡ��Լ��Ŀt��Ӧ�Ĳ���ʽ������
	void buildLR1Table();//��������������LR������
	void writeLR1TableFile(string path);//�������LR1���������

	string tranSymbol(Word w);//���ʷ��������õ��ĵ���ת��Ϊ�ķ��е��ս����
	char analyse(int status, string symbol, Word now_word);//һ�η���
	void LRAnalyse(list<Word> words);//����LR�﷨����

	vector<string> msplit(string st, char sign);//���ַ�����sign�ָ����һ������
	void readLR1Table(string path);//��ȡ����LR1��������ļ�
	void showStack();//��ʾջ������

	void error(Word w);//������


	


};

