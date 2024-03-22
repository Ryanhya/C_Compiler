
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "LexicalAnalysis.h"

using namespace std;


//��ȡ�ļ�
void LexicalAnalysis::readSourceFile(string path) {
    ifstream file;
    file.open(path, ios_base::in);
    if (!file.is_open()) {
        cout << "�޷����ļ��������ԣ�" << endl;
    }

    int i = 0;
    while (getline(file, codeLine[i]))
    {
        i++;
    }
    file.close();
}




//�ж��ַ��Ƿ�Ϊ����
bool LexicalAnalysis::isDigit(char c) {
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

//�ж��ַ��Ƿ�Ϊ��ĸ
bool LexicalAnalysis::isLetter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    else
        return false;
}

//�жϵ����Ƿ�Ϊ�ؼ���
bool LexicalAnalysis::isKey(string word) {
    int i;
    for (i = 0; i < 11; i++) {
        if (keys[i] == word)
            return true;
    }
    return false;
}

//��ʶ���Ƿ��Ѿ�����
bool LexicalAnalysis::idExisted(string name) {
    list<SymbolItem>::iterator plist;
    for (plist = sList.symbolList.begin(); plist != sList.symbolList.end(); plist++) {
        if ((*plist).name == name)
            return true;
    }
    return false;
}

//�ж��Ƿ�Ϊ�ֽ��
bool LexicalAnalysis::isSeparated(char c) {
    switch (c) {
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case ',':
    //case '\'':
        return true;
    default:
        return false;
    }
}

//�ж��Ƿ�Ϊ�����ַ����������+-*/><=!
bool LexicalAnalysis::isOperator(char c) {
    switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '>':
    case '<':
    case '=':
    case '!':
    case '&':
    case '|':
        return true;
    default:
        return false;
    }
}


//���дʷ�����,ÿ�η���һ��Դ���룬��д�����б�ͷ��ű�
void LexicalAnalysis::lexAnalysisCodeLine(string code, int line) {
    int i = 0;
    char peek;//�ַ�����
    Word word;
    string tmp = "";
    bool letterFlag = false;//�Ƿ�����ʶ���ʶ��
    bool digitFlag = false;//�Ƿ�����ʶ�����ֳ���
    bool charFlag = false;//�Ƿ�����ʶ���ַ�����
    int length = code.length();
    //cout << "����" << length << endl;
    while (i < length) {
        peek = code[i];
        //ǰ��Ĵ������Ѿ����ֶ���ע�͵�ǰ׺
        if (flag) {
            if (peek != '*') {
                i++;
                continue;
            }
            else {
                if (i + 1 < length && code[i + 1] == '/') {
                    flag = false;
                    i = i + 2;
                    cout << "����ע��ƥ��ɹ���" << endl;
                    continue;
                }
                else {
                    i++;
                    continue;
                }
            }
        }
        //��δ���ֶ���ע��
        else {
            //ֱ�������ո񡢻��е�
            if ((peek == ' ' || peek == '\n') && (!letterFlag) && (!digitFlag) && (!charFlag)) {
                i++;
                cout << "������һ�οո�" << endl;
                continue;
            }
            //���ֵ���ע�ͣ������ֱ����ֹ�������д���
            if (peek == '/' && i + 1 < length && code[i + 1] == '/') {
                cout << "���ֵ���ע��" << endl;
                break;
                //return;
            }
            //���ֶ���ע��
            if (peek == '/' && i + 1 < length && code[i + 1] == '*') {
                flag = true;
                cout << "���ֶ���ע��ǰ׺" << endl;
                i++;
                continue;//���ܶ���ע�͵ĺ�׺���ڱ��д��룬��˲����˳��Ա��д���ķ���                
            }

            //ʶ���ַ�����
            if (!charFlag) {
                if (peek == '\'') {
                    charFlag = true;
                    i++;
                    /*word.type = 5;
                    word.value = peek;
                    word.line = line;
                    word.column = i + 1;
                    wList.wordList.push_back(word);*/
                    continue;
                }
            }
            else {
                if (isLetter(peek) && i + 1 < length && code[i + 1] == '\'') {
                    tmp += peek;
                    cout << tmp << endl;
                    word.type = 3;
                    word.value = tmp;
                    word.line = line;
                    word.column = i + 1;
                    wList.wordList.push_back(word);//д�뵥���б�
                    charFlag = false;
                    tmp = "";                   
                    i = i + 2;
                    continue;
                }
                else {
                    //������
                }
            }

            //ʶ���ʶ����ؼ���
            if (!letterFlag) {
                if (isLetter(peek)) {
                    letterFlag = true;//��ʼʶ���ʶ����ؼ��� 
                    tmp += peek;
                    i++;
                    continue;
                }
            }
            else {
                if (isLetter(peek) || isDigit(peek)) {
                    tmp += peek;
                    i++;
                    continue;
                }
                else {

                    cout << tmp << endl;
                    //����Ϊ�ؼ���
                    if (isKey(tmp)) {
                        word.type = 4;
                        word.value = tmp;
                        word.line = line;
                        word.column = i;
                        wList.wordList.push_back(word);//д�뵥���б�
                    }
                    //����Ϊ��ʶ��
                    else {

                        word.type = 1;
                        word.value = tmp;
                        word.line = line;
                        word.column = i;
                        wList.wordList.push_back(word);//д�뵥���б�
                        if (!idExisted(tmp)) {
                            SymbolItem sym;
                            sym.name = tmp;//��ʶ����
                            sym.line = line;//����Դ��������                       
                            sList.symbolList.push_back(sym);//��д���ű�
                        }
                    }
                    letterFlag = false;
                    tmp = "";//����Ա�洢��һ����������
                    continue;
                }
            }

            //ʶ�����ֳ���
            if (!digitFlag) {
                if (isDigit(peek)) {
                    digitFlag = true;
                    tmp += peek;
                    i++;
                    continue;
                }
            }
            else {
                if (isDigit(peek)) {
                    tmp += peek;
                    i++;
                    continue;
                }
                else {
                    cout << tmp << endl;
                    word.type = 2;
                    word.value = tmp;
                    word.line = line;
                    word.column = i;
                    wList.wordList.push_back(word);//д�뵥���б�
                    digitFlag = false;
                    tmp = "";
                    continue;
                }
            }


            //�����
            if (isOperator(peek)) {
                tmp += peek;
                word.type = 6;
                word.line = line;
                switch (peek) {
                case '+':
                    if (i + 1 < length && code[i + 1] == '+') {
                        tmp += '+';
                        word.value = tmp;
                        word.column = i + 2;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '-':
                    if (i + 1 < length && code[i + 1] == '-') {
                        tmp += '-';
                        word.value = tmp;
                        word.column = i + 2;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '*':
                    word.value = tmp;
                    word.column = i + 1;
                    wList.wordList.push_back(word);
                    i++;
                    break;
                case '/':
                    word.value = tmp;
                    word.column = i + 1;
                    wList.wordList.push_back(word);
                    i++;
                    break;
                case '=':
                    if (i + 1 < length && code[i + 1] == '=') {
                        tmp += '=';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '>':
                    if (i + 1 < length && code[i + 1] == '=') {
                        tmp += '=';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '<':
                    if (i + 1 < length && code[i + 1] == '=') {
                        tmp += '=';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '!':
                    if (i + 1 < length && code[i + 1] == '=') {
                        tmp += '=';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '&':
                    if (i + 1 < length && code[i + 1] == '&') {
                        tmp += '&';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                case '|':
                    if (i + 1 < length && code[i + 1] == '|') {
                        tmp += '|';
                        word.column = i + 2;
                        word.value = tmp;
                        i = i + 2;
                        wList.wordList.push_back(word);
                    }
                    else {
                        word.value = tmp;
                        word.column = i + 1;
                        wList.wordList.push_back(word);
                        i++;
                    }
                    break;
                }
                cout << tmp << endl;
                tmp = "";
                continue;
            }

            //�ֽ��
            if (isSeparated(peek)) {
                tmp += peek;
                word.type = 5;
                word.value = tmp;
                word.line = line;
                word.column = i + 1;
                wList.wordList.push_back(word);
                cout << tmp << endl;
                tmp = "";
                i++;
                continue;
            }


        }

    }
}




