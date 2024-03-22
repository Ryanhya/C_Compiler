
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "LexicalAnalysis.h"

using namespace std;


//读取文件
void LexicalAnalysis::readSourceFile(string path) {
    ifstream file;
    file.open(path, ios_base::in);
    if (!file.is_open()) {
        cout << "无法打开文件，请重试！" << endl;
    }

    int i = 0;
    while (getline(file, codeLine[i]))
    {
        i++;
    }
    file.close();
}




//判断字符是否为数字
bool LexicalAnalysis::isDigit(char c) {
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}

//判断字符是否为字母
bool LexicalAnalysis::isLetter(char c) {
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    else
        return false;
}

//判断单词是否为关键字
bool LexicalAnalysis::isKey(string word) {
    int i;
    for (i = 0; i < 11; i++) {
        if (keys[i] == word)
            return true;
    }
    return false;
}

//标识符是否已经存在
bool LexicalAnalysis::idExisted(string name) {
    list<SymbolItem>::iterator plist;
    for (plist = sList.symbolList.begin(); plist != sList.symbolList.end(); plist++) {
        if ((*plist).name == name)
            return true;
    }
    return false;
}

//判断是否为分界符
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

//判断是否为单个字符的运算符即+-*/><=!
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


//进行词法分析,每次分析一行源代码，填写单词列表和符号表
void LexicalAnalysis::lexAnalysisCodeLine(string code, int line) {
    int i = 0;
    char peek;//字符变量
    Word word;
    string tmp = "";
    bool letterFlag = false;//是否正在识别标识符
    bool digitFlag = false;//是否正在识别数字常量
    bool charFlag = false;//是否正在识别字符常量
    int length = code.length();
    //cout << "长度" << length << endl;
    while (i < length) {
        peek = code[i];
        //前面的代码行已经出现多行注释的前缀
        if (flag) {
            if (peek != '*') {
                i++;
                continue;
            }
            else {
                if (i + 1 < length && code[i + 1] == '/') {
                    flag = false;
                    i = i + 2;
                    cout << "多行注释匹配成功！" << endl;
                    continue;
                }
                else {
                    i++;
                    continue;
                }
            }
        }
        //尚未出现多行注释
        else {
            //直接跳过空格、换行等
            if ((peek == ' ' || peek == '\n') && (!letterFlag) && (!digitFlag) && (!charFlag)) {
                i++;
                cout << "跳过了一次空格" << endl;
                continue;
            }
            //出现单行注释，则可以直接终止分析本行代码
            if (peek == '/' && i + 1 < length && code[i + 1] == '/') {
                cout << "出现单行注释" << endl;
                break;
                //return;
            }
            //出现多行注释
            if (peek == '/' && i + 1 < length && code[i + 1] == '*') {
                flag = true;
                cout << "出现多行注释前缀" << endl;
                i++;
                continue;//可能多行注释的后缀就在本行代码，因此不能退出对本行代码的分析                
            }

            //识别字符常量
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
                    wList.wordList.push_back(word);//写入单词列表
                    charFlag = false;
                    tmp = "";                   
                    i = i + 2;
                    continue;
                }
                else {
                    //出错处理
                }
            }

            //识别标识符或关键字
            if (!letterFlag) {
                if (isLetter(peek)) {
                    letterFlag = true;//开始识别标识符或关键字 
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
                    //单词为关键字
                    if (isKey(tmp)) {
                        word.type = 4;
                        word.value = tmp;
                        word.line = line;
                        word.column = i;
                        wList.wordList.push_back(word);//写入单词列表
                    }
                    //单词为标识符
                    else {

                        word.type = 1;
                        word.value = tmp;
                        word.line = line;
                        word.column = i;
                        wList.wordList.push_back(word);//写入单词列表
                        if (!idExisted(tmp)) {
                            SymbolItem sym;
                            sym.name = tmp;//标识符名
                            sym.line = line;//所属源代码行数                       
                            sList.symbolList.push_back(sym);//填写符号表
                        }
                    }
                    letterFlag = false;
                    tmp = "";//清空以便存储下一个单词内容
                    continue;
                }
            }

            //识别数字常量
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
                    wList.wordList.push_back(word);//写入单词列表
                    digitFlag = false;
                    tmp = "";
                    continue;
                }
            }


            //运算符
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

            //分界符
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




