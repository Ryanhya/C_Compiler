// C_Compiler.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <string>
#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"


using namespace std;

//全局变量

const string rpath = "source/source.txt";//读取的源代码文件路径
const string wordListPath = "output/word_list.txt";//写单词列表文件路径
const string symbolListPath = "output/symbol_list.txt";//写符号表文件路径
const string grammarPath = "source/extended_grammar.txt";//读取文法产生式文件的路径
const string firstPath = "output/First.txt";//写First集文件路径
const string followPath = "output/Follow.txt";//写Follow集文件路径
const string LR1CollectionsResultPath = "output/LR1CollectionsResult.txt";//存储LR1项目规范簇自动构造的结果
const string LR1TablePath = "output/LR1Table.txt";//存储LR1分析表


int main()
{
    //词法分析器
    Word end_word;//结束符
    end_word.type = 5;
    end_word.value = "$";
    LexicalAnalysis* myLex = new LexicalAnalysis();
    myLex->readSourceFile(rpath);//读取源代码文件
    for (int i = 0; i < N; i++) {
        myLex->lexAnalysisCodeLine(myLex->codeLine[i], i + 1);
    }
    myLex->wList.wordList.push_back(end_word);//写入结束符$
    myLex->wList.writeWordListFile(wordListPath);//输出单词列表文件
    myLex->sList.writeSymbolListFile(symbolListPath);//输出符号表文件


    cout << endl << endl << endl;

    //语法分析器
    SyntaxAnalysis* mySyntax=new SyntaxAnalysis();
    vector<string> v;
    mySyntax->readGrammarFile(grammarPath);
    /*for (int i = 0; i < mySyntax.grammar.size(); i++) {
        v = mySyntax.splitRight(mySyntax.grammar[i].pright);
        for (int j = 0; j < v.size(); j++)
            cout << v[j] << endl;
    }*/
    
    //计算First集和Follow集
    mySyntax->getFirst();
    mySyntax->writeFirstFile(firstPath);
    mySyntax->getFollow();
    mySyntax->writeFollowFile(followPath);
    
    /*Term t;
    t.pleft = "S";
    t.pright = "void main(){Y}";
    t.right = { "void","main","(",")","{","Y","}" };
    t.dot = 4;
    t.forward = "$";
    
    vector<Term> s= mySyntax.closure(t);
    int i = 0;
    for (auto it = s.begin(); it != s.end(); it++) {
        cout << (*it).pleft << "  " << (*it).pright << "  " << (*it).forward << endl;
        i++;
    }
    cout << i;*/   

    /*t.pleft = "S";
    t.pright = "void main(){Y}";
    t.right = { "void","main","(",")","{","Y","}" };
    t.dot = 4;
    t.forward = "$";*/

    Term t;
    t.pleft = "S\'";
    t.pright = "S";
    t.right = { "S" };
    t.dot = -1;
    t.forward = "$";    
   
    //自动构造LR1分析表，原文法一次构造约为七分半，加入制导翻译后更改的文法约九分钟
    /*mySyntax->buildLR1Collections(t);
    mySyntax->writeLR1ResultFile(LR1CollectionsResultPath);
    mySyntax->buildLR1Table();
    mySyntax->writeLR1TableFile(LR1TablePath);*/


    //读取本地存储的LR1分析表
    mySyntax->readLR1Table(LR1TablePath);

    //传入词法分析器获取到的单词列表，进行LR1语法分析
    mySyntax->LRAnalyse(myLex->wList.wordList);



    //语义分析器



    //释放内存
    delete(myLex);
    delete(mySyntax);

   
   



    





    

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
