#pragma once
#include <string>
#include <vector>

using namespace std;


//������ݽṹ
class Term {
public:
	bool is_core = false;//��Ŀ�Ƿ�Ϊ������Ŀ
	int type;//��Ŀ���ͣ���S->a.bA,#����1��ʾ�ƽ���Ŀ��2��ʾ��Լ��Ŀ����S->A.,#����3��ʾ��Լ��Ŀ
	string pleft;//����ʽ��
	string pright;//����ʽ�Ҳ�
	vector<string> right;//����ʽ�Ҳ�
	int dot = -1;//��ǰ׺λ�ã�dotֵ��һ��Ϊ��һ�����ŵ�����
	string forward;//��ǰ������
	bool termEqual(Term t);//�жϵ�ǰTerm��t�Ƿ����
};

