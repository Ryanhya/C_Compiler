#pragma once
#include <string>


using namespace std;

class Reflection {
public:
	int start;//ǰһ��״̬�ı��
	int end;//��һ��״̬�ı��
	string received_symbol;//��״̬startת�Ƶ�״̬end��Ҫ���յķ���
	int type;//1Ϊ��ʾ���յķ���Ϊ�ս���ţ�2��ʾ���յķ���Ϊ���ս���ţ��ڹ��������ʱ�����ж���ACTION����GOTO
};