#pragma once
#include <string>


using namespace std;


class Word {
public:
    int type;//�������ͣ�1Ϊ��ʶ����2Ϊ���ֳ�����3Ϊ�ַ�������4Ϊ�ؼ��֣�5Ϊ�ֽ����6Ϊ�����
    string value;//���ʵ�ֵ
    int line;//����Դ���������
    int column;//���ʵ����һ���ַ�����Դ���������
    Word() :type(0), value(""), line(0), column(0) {}//���캯��
};