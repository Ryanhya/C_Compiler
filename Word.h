#pragma once
#include <string>


using namespace std;


class Word {
public:
    int type;//单词类型，1为标识符，2为数字常量，3为字符常量，4为关键字，5为分界符，6为运算符
    string value;//单词的值
    int line;//所在源程序的行数
    int column;//单词的最后一个字符所在源程序的列数
    Word() :type(0), value(""), line(0), column(0) {}//构造函数
};