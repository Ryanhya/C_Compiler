#pragma once
#include <string>


using namespace std;

class SymbolItem {
public:
    string name;//名称
    int type;//数据类型，1为int，2为char，3为bool
    int attribute;//种类，1为常量，2为变量
    int storageType;//存储类型，具体含义后续完善...
    int level;//符号所属的层次
    int addr;//地址码
    int line;//标识符声明语句在源程序中的行号信息
    SymbolItem() :name(""), type(0), attribute(0), storageType(0), level(0), addr(0), line(0) {}//构造函数


};