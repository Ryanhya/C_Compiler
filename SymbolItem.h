#pragma once
#include <string>


using namespace std;

class SymbolItem {
public:
    string name;//����
    int type;//�������ͣ�1Ϊint��2Ϊchar��3Ϊbool
    int attribute;//���࣬1Ϊ������2Ϊ����
    int storageType;//�洢���ͣ����庬���������...
    int level;//���������Ĳ��
    int addr;//��ַ��
    int line;//��ʶ�����������Դ�����е��к���Ϣ
    SymbolItem() :name(""), type(0), attribute(0), storageType(0), level(0), addr(0), line(0) {}//���캯��


};