#pragma once
#include <string>


using namespace std;

class Reflection {
public:
	int start;//前一个状态的标号
	int end;//后一个状态的标号
	string received_symbol;//从状态start转移到状态end需要接收的符号
	int type;//1为表示接收的符号为终结符号，2表示接收的符号为非终结符号，在构造分析表时用以判断填ACTION还是GOTO
};