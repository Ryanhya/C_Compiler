#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <queue>
#include "SyntaxAnalysis.h"
#include "SemanticAnalysis.h"


using namespace std;


bool SyntaxAnalysis::isVt(string str) {
    int i;
    for (i = 0; i < vt_num; i++) {
        if (Vt[i] == str)
            return true;
    }
    return false;
}


set<int> SyntaxAnalysis::emptyIndex(){
    int i;
    set<int> tmp;
    for (i = 0; i < grammar.size(); i++) {
        if (grammar[i].pright == "#") {
            tmp.insert(i);
        }
    }
    return tmp;
}

//获取文法中的终结符号
string SyntaxAnalysis::getVt(string production) {
    int k;

    if (production.substr(0, 9) == "character") {
        return "character";
    }
    else if (production.substr(0, 3) == "num") {
        return "num";
    }
    else if (production.substr(0, 2) == "id") {
        return "id";
    }
    
   
    //优先匹配双字符，防止要匹配>=时匹配到>就退出
    for (k = 3; k < 27; k++)
    { //这里应该注意：使用最长匹配
        
        if (production.substr(0, 2) == Vt[k])
            return production.substr(0, 2);
    }

    for (k = 3; k < 27; k++)
    {
        if (production.substr(0, 1) == Vt[k])
            return Vt[k];
    }
   

    //匹配关键字
    string tmp;
    for (k = 27; k < vt_num; k++)
    {
        tmp = production.substr(0, Vt[k].length());
        if (tmp == Vt[k])
        {
            return tmp;
        }
    }
    //如果运行到这里，说明这个终结符不是终结符号表里的，认为小写字母也属于终结符
    /*if (production[0] >= 'a' && production[0] <= 'z')
    {
        return production.substr(0, 1);
    }*/
    
}


//将一条产生式的右部划分成非终结符号和终结符号的组合
vector<string> SyntaxAnalysis::splitRight(string right) {
    
    vector<string> v;
    int j;
    int length = right.length();

    for (j = 0; j < length; j++)
    {
        //跳过空格
        if (right[j] == ' ')
        {
            continue;
        }
        if (right[j] >= 'A' && right[j] <= 'Z')
        { //非终结符
            string Vn="";
            if ((j == length - 1) || (j + 1 < length && right[j + 1] != '\''))//处理类似于S'的非终结符号
                Vn = Vn + right[j];
            else 
                Vn = Vn + right[j] + '\'';
            v.push_back(Vn);
            j = j + Vn.length() - 1;
            Vn.clear();
        }
        else
        { //终结符
            string vt;
            vt = getVt(right.substr(j, length - j));
            v.push_back(vt);
            j = j + vt.length() - 1;
            vt.clear();
        }
    }
    return v;
   
}

//读取文法产生式
void SyntaxAnalysis::readGrammarFile(string path) {
	ifstream file;
    file.open(path, ios_base::in);
    if (!file.is_open()) {
        cout << "无法打开文件，请重试！" << endl;
    }

    string str;
    Production p;
    vector<string> tmp;
    while (getline(file, str)) {
        if (str[1] == '-') {
            tmp = SyntaxAnalysis::splitRight(str.substr(3, str.length() - 3));
            p.pleft = str[0];
            p.pright = str.substr(3, str.length() - 3);
            p.right = tmp;
            grammar.push_back(p);
            //读文件时便可以创建非终结符号集Vn
            Vn.insert(p.pleft);
        }
        //处理带有'的非终结符号
        else {
            tmp = SyntaxAnalysis::splitRight(str.substr(4, str.length() - 4));
            p.pleft = str.substr(0,2);
            p.pright = str.substr(4, str.length() - 4);
            p.right = tmp;
            grammar.push_back(p);
            //读文件时便可以创建非终结符号集Vn
            Vn.insert(p.pleft);
        }
    }

    int i = 0;
    for (auto it = Vn.begin(); it != Vn.end(); it++) {
        myVn[i] = *it;
        i++;
    }

    file.close();
}


//递归求单个symbol的First集
set<string> SyntaxAnalysis::getSymbolFirst(string symbol) {
    set<string> tmp;
    //bool emptyFlag = false;//symbol是否能推导为空
    //终结符号的First集为它本身
    if (isVt(symbol)) {
        tmp.insert(symbol);
        return tmp;
    }
    else {//symbol为非终结符号
        int i;
        //遍历产生式集合
        for (i = 0; i < grammar.size(); i++) {
            //左部为symbol的产生式
            if (grammar[i].pleft == symbol) {
                //若为左递归则跳过该产生式
                if (grammar[i].pleft == grammar[i].right[0])
                    continue;
                string fir_right = grammar[i].right[0];
                //产生式右部第一个符号为终结符号
                if (isVt(fir_right)) {
                    tmp.insert(fir_right);//加入First集中
                    //若symbol能推导为空
                    //if (emptyFlag) {
                    //    //tmp.
                    //}

                }
                else {
                    //set<int> s;
                    set<string> next;
                    //s = emptyIndex();
                    //检验fir_right能否推导为空
                    //for (auto it = s.begin(); it != s.end(); it++) {
                        //if (grammar[*it].pleft == fir_right) {
                            //emptyFlag = true;
                            //break;
                        //}
                    //}
                    next = getSymbolFirst(fir_right);
                    tmp.insert(next.begin(), next.end());//将tmp与next合并
                }
            }
        }
        return tmp;
    }
}




void SyntaxAnalysis::getFirst() {
    set<string> s;
    int i, j;
    //先建立终结符号的First集，即为它自身
    for (i = 0; i < vt_num; i++) {
        s.insert(Vt[i]);
        allFirst[Vt[i]].insert(s.begin(), s.end());
        s.clear();
    }
    for (i = 0; i < grammar.size(); i++) {
        for (j = 0; j < grammar[i].right.size(); j++) {
            s = getSymbolFirst(grammar[i].right[j]);
            //s只包含空时，求产生式右部的下一个符号的First集
            if (!(s.size() == 1 && s.count("#") == 1))
                break;
        }
        allFirst[grammar[i].pleft].insert(s.begin(), s.end());
        allFirst[grammar[i].pright].insert(s.begin(), s.end());//set合并
    }
}

//求非终结符号symbol的Follow集
set<string> SyntaxAnalysis::getSymbolFollow(string symbol) {
    set<string> s;
    int i;
    //若symbol为开始符号，结束符$加入其Follow集
    if (symbol == start_symbol) {
        s.insert("$");
        return s;
    }


    //遍历产生式集合
    for (i = 0; i < grammar.size(); i++) {
        //symbol不在第i条产生式的右部则在下一条产生式中寻找
        if (grammar[i].pright.find(symbol) == grammar[i].pright.npos)
            continue;
        else {
            int j, index, k;
            string next;

            for (j = 0; j < grammar[i].right.size(); j++) {
                if (grammar[i].right[j] == symbol) {
                    index = j + 1;//下一个符号的索引
                    set<string> tmp;
                    //恰好为右部最后一个符号
                    if (index == grammar[i].right.size()) {
                        if (grammar[i].pleft == symbol)
                            continue;//若该条产生式左部恰好为symbol，FOLLOW(symbol)属于FOLLOW(symbol)无意义，找该条产生式右部的下一个符号
                        //tmp = getSymbolFollow(grammar[i].pleft);//求产生式左部的FOLLOW集(会因栈溢出而报错）
                        tmp = allFollow[grammar[i].pleft];//要求左部的FOLLOW集在求symbol的FOLLOW集前已经求出
                        s.insert(tmp.begin(), tmp.end());
                    }
                    else {
                        for (k = index; k < grammar[i].right.size(); k++) {
                            next = grammar[i].right[k];
                            tmp = allFirst[next];//求其First集
                            if (!tmp.count("#")) {
                                s.insert(tmp.begin(), tmp.end());
                                break;//不含空则可以直接跳出对后面First集的求解
                            }
                            else {
                                tmp.erase("#");
                                s.insert(tmp.begin(), tmp.end());
                            }
                            if (k == grammar[i].right.size() - 1) {//指针指向右部最后一个符号且之前仍未退出该循环体
                                if (grammar[i].pleft == symbol)
                                    break;//若该条产生式左部恰好为symbol，FOLLOW(symbol)属于FOLLOW(symbol)无意义，找该条产生式右部的下一个符号
                                //tmp = getSymbolFollow(grammar[i].pleft);//求产生式左部的FOLLOW集(会因栈溢出而报错）
                                tmp = allFollow[grammar[i].pleft];//要求左部的FOLLOW集在求symbol的FOLLOW集中已经求出                              
                                s.insert(tmp.begin(), tmp.end());
                                
                            }

                        }
                    }

                }

            }
            

        }

    }   

    return s;

}


void SyntaxAnalysis::getFollow() {
    set<string> s;
    int i;
    //计算所有产生式左部的Follow集,因为非终结符号一定在左部
    for (i = 0; i < grammar.size(); i++) {
        s = getSymbolFollow(grammar[i].pleft);
        allFollow[grammar[i].pleft].insert(s.begin(), s.end());
    }
    //特殊处理存在的环路
    allFollow["G"].insert(allFollow["J"].begin(), allFollow["J"].end());
    allFollow["G"].insert(allFollow["K"].begin(), allFollow["K"].end());
}






void SyntaxAnalysis::writeFirstFile(string path) {
    ofstream file;
    file.open(path, ios::out);
    for (auto i = allFirst.begin(); i != allFirst.end(); i++) {
        file << "First(" << i->first << ") = {  ";
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            file << *j << "  ";
        }
        file << "}" << endl;
    }
    file.close();
}



void SyntaxAnalysis::writeFollowFile(string path) {
    ofstream file;
    file.open(path, ios::out);
    for (auto i = allFollow.begin(); i != allFollow.end(); i++) {
        file << "Follow(" << i->first << ") = {  ";
        for (auto j = i->second.begin(); j != i->second.end(); j++) {
            file << *j << "  ";
        }
        file << "}" << endl;
    }
    file.close();
}


int SyntaxAnalysis::setTermType(Term t) {
    int nextIndex = t.dot + 1;//下一个要识别的符号的索引
    //存在下一个符号
    if (nextIndex < t.right.size()) {
        //终结符号
        if (isVt(t.right[nextIndex]) && t.right[nextIndex] != "#") {
            return 1;
        }
        else if(t.right[nextIndex] == "#"){
            return 2;
        }
        else {
            return 3;//待约项目
        }
    }
    else {
        return 2;//为归约项目
    }
}


//初始化一个LR1项目为其核心项目Ino：Term1、Term2...
void SyntaxAnalysis::initialTermCollection(int no, vector<Term> v) {
    int i;
    for (i = 0; i < v.size(); i++) {
        v[i].type = setTermType(v[i]);//设置类型信息
        v[i].is_core = true;//标注为核心项目
        LR1_collection[no].push_back(v[i]);
        flags.insert(pair<int, bool>(no, false));//该状态未派生
        
    }
    //LR1_collection[name].insert(v.begin(), v.end());
}



bool SyntaxAnalysis::isTermExist(Term t, vector<Term> v) {
    bool flag = false;
    int m;
    for (m = 0; m < v.size(); m++) {               
        if (t.termEqual(v[m])) {
            flag = true;
            break;
        }                       
    }
    return flag;
}


//计算待约项目term的闭包
vector<Term> SyntaxAnalysis::closure(Term term) {
    vector<Term> s;
    int nextSymbolIndex = term.dot + 1;   
    string nextSymbol = term.right[nextSymbolIndex];

    Term tmp;
    int i;
    string str;
    set<string> first;
    set<string> forward_set;//向前搜索符集合
    
    //nextSymbol为term的右部最后一个符号
    if (nextSymbolIndex + 1 == term.right.size()) {
        forward_set.insert(term.forward);
    }
    else {
        for (i = nextSymbolIndex + 1; i < term.right.size(); i++) {
            str = term.right[i];
            first = allFirst[str];
            forward_set.insert(first.begin(), first.end());
            //当前符号str的First集不含空#，则向前搜索符计算完成
            if (!first.count("#")) {
                forward_set.erase("#");
                break;
            }
        }
        if (forward_set.count("#")) {
            forward_set.erase("#");
            forward_set.insert(term.forward);
        }
    }

    //遍历产生式集，寻找左部为nextSymbol的产生式
    for (i = 0; i < grammar.size(); i++) {
        if (grammar[i].pleft != nextSymbol)
            continue;
        tmp.pleft = grammar[i].pleft;
        tmp.pright = grammar[i].pright;
        tmp.right = grammar[i].right;
        for (auto j = forward_set.begin(); j != forward_set.end(); j++) {
            tmp.forward = (*j);
            s.push_back(tmp);
        }

    }                               
    return s;
}






void SyntaxAnalysis::extendTerm(int no) {
    vector<Term> v;
    v = LR1_collection[no];//获取核心项目
    int i, j;   
    Term t;
    queue<Term> q;//待约项目队列
    vector<Term> temp;//存储核心项目求闭包所得的派生项目
    
    
    for (i = 0; i < v.size(); i++) {
        //若为归约项目或移进项目，不用派生
        if (v[i].type != 3)
            continue;
        q.push(v[i]);                      
    }

    //BFS求每一层的待约项目
    while (!q.empty()) {
        t = q.front();//取队首
        q.pop();//出队
        temp = closure(t);
        for (j = 0; j < temp.size(); j++) {
            if (isTermExist(temp[j], LR1_collection[no]))
                continue;
            else {
                temp[j].type = setTermType(temp[j]);
                LR1_collection[no].push_back(temp[j]);
                if (temp[j].type == 3)
                    q.push(temp[j]);
            }
        }
    }
    flags[no] = true;//派生完成

}

bool SyntaxAnalysis::isCoreTermExist(vector<Term> v) {
    int i;
    int nowIndex;
    bool flag = false;
    vector<Term> collection;//当前状态的核心项目
    vector<Term> nowCollection;//当前状态的全部项目
    //遍历当前已有的状态
    for (auto it = LR1_collection.begin(); it != LR1_collection.end(); it++) {
        nowIndex = (*it).first;
        nowCollection = (*it).second;
        //将状态的核心项目加入collection  
        if (flags[nowIndex]) {
            for (i = 0; i < nowCollection.size(); i++) {
                //若为核心项目
                if (nowCollection[i].is_core)
                    collection.push_back(nowCollection[i]);
            }
        }
        else
            collection = nowCollection;
        

        if (collection.size() >= v.size()) {
            for (i = 0; i < v.size(); i++) {
                //有一项不存在，则项目集必然不一样，找下一个已有的状态
                if (!isTermExist(v[i], collection))
                    break;
            }
            if (i == v.size()) {//之前均未跳出，则说明v和collection完全匹配上，v已存在
                flag = true;
                pre = nowIndex;
                break;
            }
        }
        
    }
    return flag;
}


bool SyntaxAnalysis::isFlagsAllT() {
    bool myflag;
    for (auto it = flags.begin(); it != flags.end(); it++) {
        myflag = (*it).second;
        //若有一个为false，直接返回false
        if (!myflag)
            return false;
    }
    return true;
}



void SyntaxAnalysis::getNewCollection(int no) {
    unordered_map<string, vector<Term>> new_collections;//从当前状态no接收string后到达新状态的核心项目
    vector<Term> v = LR1_collection[no];//获得状态no经过派生后的全部项目
    int i;
    int index;
    string nextSym;
    vector<Term> tmp;
    Reflection r;

    for (i = 0; i < v.size(); i++) {        
        if (v[i].type == 2)//归约项目不用移动活前缀
            continue;       
        else {//待约或移进项目
            index = v[i].dot;
            nextSym = v[i].right[index + 1];//活前缀要识别的下一个符号
            v[i].dot = index + 1;//活前缀移进一位
            new_collections[nextSym].push_back(v[i]);            
        }
        
    }

    for (auto it = new_collections.begin(); it != new_collections.end(); it++) {
        nextSym = (*it).first;
        tmp = (*it).second;
        //核心项目已经出现过
        if (isCoreTermExist(tmp)) {
            r.start = no;
            r.end = pre;
            r.received_symbol = nextSym;
            if (isVt(nextSym))
                r.type = 1;
            else
                r.type = 2;
            reach.push_back(r);
            continue;
        }
        else {
            num = num + 1;
            initialTermCollection(num, tmp);
            r.start = no;
            r.end = num;
            r.received_symbol = nextSym;
            if (isVt(nextSym))
                r.type = 1;
            else
                r.type = 2;
            reach.push_back(r);
            
        }
    }
    
}


//根据开始符号产生式的LR项目，构造LR(1)项目规范簇
void SyntaxAnalysis::buildLR1Collections(Term start) {
    vector<Term> st;
    int i = 0;
    st.push_back(start);
    initialTermCollection(i, st);
    do {
        extendTerm(i);
        getNewCollection(i);
        i++;
    } while (!isFlagsAllT());
}



void SyntaxAnalysis::writeLR1ResultFile(string path) {
    ofstream file;
    file.open(path, ios::out);

    int num, i, j;
    vector<Term> v;
    Term t;

    for (auto it = LR1_collection.begin(); it != LR1_collection.end(); it++) {
        num = (*it).first;
        v = (*it).second;
        file << "I" << num << ":" << endl;
        for (i = 0; i < v.size(); i++) {
            t = v[i];
            if (t.dot + 1 < t.right.size()) {
                file << "[ " << t.pleft << "-> ";
                for (j = 0; j < t.right.size(); j++) {
                    if (j == t.dot + 1)
                        file << ".";
                    file << t.right[j];
                }
                file<< " , " << t.forward << " ]" << "  ";
            }
            else
                file << "[ " << t.pleft << "-> " << t.pright.append(".") << " , " << t.forward << " ]" << "  ";
        }
        file << endl;
    }
    file << endl;
    
    Reflection r;
    //写入状态间的映射关系
    for (i = 0; i < reach.size(); i++) {
        r = reach[i];
        file << "t( " << r.start << " , " << r.received_symbol << " )=" << r.end << endl;
    }
    file.close();


}

int SyntaxAnalysis::vt2int(string s) {
    int i;
    for (i = 0; i < vt_num; i++) {
        if (s == Vt[i])
            return i;
    }
    return -1;//不存在该终结符号
}


int SyntaxAnalysis::vn2int(string s) {
    int i;
    for (i = 0; i < vn_num; i++) {
        if (myVn[i] == s)
            return i;
    }
    return -1;//不存在该非终结符号

}

//进行归约时获取产生式下标
int SyntaxAnalysis::getProductionIndex(Term t) {
    int i;
    for (i = 0; i < grammar.size(); i++) {
        if (grammar[i].pleft == t.pleft && grammar[i].pright == t.pright)
            return i;
    }
    return -1;//未找到
}




void SyntaxAnalysis::buildLR1Table() {
    int i, k, m, index;
    int j = 0;
    vector<Term> v;
    Reflection r;

    for (auto it = LR1_collection.begin(); it != LR1_collection.end(); it++) {
        i = (*it).first;
        v = (*it).second;
        while (j < reach.size()) {
            r = reach[j];            
            if (r.start == i) {
                if (r.type == 1) {
                    k = vt2int(r.received_symbol);//在数组Vt的下标
                    ACTION[i][k] = "S" + to_string(r.end);//移进
                }
                else {
                    k = vn2int(r.received_symbol);//在数组myVn的下标
                    GOTO[i][k] = r.end;
                }
            }
            else {
                break;//reach每一项按其start值从小到大排列，遇到不相同的说明其start大于i，等外层到达start值时再循环
            }
            j++;

        }


        //填写归约项目，归约项目（除推出空#）一定在状态的核心项目中，而一个状态的核心项目排在其项目集的前面
        for (m = 0; m < v.size(); m++) {
            ////若能推出空
            //if (v[m].pright == "#") {
            //    index = getProductionIndex(v[m]);
            //    k = vt2int(v[m].forward);//在数组Vt中的下标
            //    ACTION[i][k]= "r" + to_string(index);//空产生式归约
            //    continue;
            //}
            //if (!v[m].is_core)//不是核心项目，说明已经查询过该状态所有的核心项目
            //    break;
            if (v[m].type == 2) {
                //查询对应的产生式索引
                index = getProductionIndex(v[m]);
                k = vt2int(v[m].forward);//在数组Vt中的下标
                if (v[m].pleft=="S\'") {
                    ACTION[i][k] = "acc";
                }
                else
                    ACTION[i][k] = "r" + to_string(index);//归约
            }
        }

        //未填充的地方为出错
        for (m = 0; m < vt_num; m++) {
            if (ACTION[i][m].empty())
                ACTION[i][m] = "ER";//出错
        }

        

    }
}





void SyntaxAnalysis::writeLR1TableFile(string path) {
    ofstream file;
    file.open(path, ios::out);

    int i, j;
    
   
    
    for (i = 0; i < 300; i++) {
        //file << i << ": ";
        for (j = 0; j < vt_num; j++) {
            file << ACTION[i][j] << " ";
        }
        for (j = 0; j < vn_num; j++) {
            file << GOTO[i][j] << " ";
        }
        file << endl;
    }
    


    file.close();
}

string SyntaxAnalysis::tranSymbol(Word w) {
    string symbol;
    //标识符
    if (w.type == 1)
        symbol = "id";
    //数字常量
    else if (w.type == 2)
        symbol = "num";
    //字符常量
    else if (w.type == 3)
        symbol = "character";
    //关键字、运算符、分界符等
    else
        symbol = w.value;

    return symbol;
}



void SyntaxAnalysis::error(Word w) {
    if (w.value != "$")
        cout << "第" << w.line << "行，第" << w.column << "列出现语法错误！请检查源代码！" << endl;
    else
        cout << "源代码结尾出错！" << endl;
}



char SyntaxAnalysis::analyse(int status, string symbol, Word now_word) {
    int index;//symbol对应的索引
    index = vt2int(symbol);
    string action;
    action = ACTION[status][index];

    int number;//ACTION表的元素的数字
    

    if (action == "ER") {
        //出错
        //cout << "发生错误！" << endl;
        showStack();
        error(now_word);
        return 'E';
    }
    //移进
    else if (action.find("S") != string::npos) {
        number = atoi(action.substr(1).c_str());//移进的状态
        statusStack.push(number);
        symbolStack.push(symbol);        
        cout << "移进" << action << endl;
        showStack();
        return 'S';
    }
    //归约
    else if (action.find("r") != string::npos) {
        int go;//GOTO表中对应的状态
        int vnindex;//产生式左部符号的索引
        int l;//产生式右部的长度
        int tmp;//当前栈顶状态
        number = atoi(action.substr(1).c_str());//产生式的索引
        if (grammar[number].pright != "#")
            l = grammar[number].right.size();
        else
            l = 0;
        for (int i = 0; i < l; i++) {
            statusStack.pop();
            symbolStack.pop();
        }
        symbolStack.push(grammar[number].pleft);//产生式左部压入符号栈中
        vnindex = vn2int(grammar[number].pleft);
        tmp = statusStack.top();
        go = GOTO[tmp][vnindex];
        statusStack.push(go);//压入状态栈

        showStack();
        cout << "下个要接受的符号：" << symbol << endl;
        cout << "使用的产生式为：" << grammar[number].pleft << "->" << grammar[number].pright << "  ";
        cout << "在状态" << tmp << "时面对" << grammar[number].pleft << "转移至状态" << go << "  ";
        cout << "归约" << action << endl;
        return 'r';
    }
    //分析成功，退出
    else if (action == "acc") {
        showStack();
        cout << "接收acc，语法分析成功！" << endl;
        return 'A';
    }

    

}


void SyntaxAnalysis::showStack() {
    string s;
    int i;
    stack<string> s1;
    stack<int> s2;
    s1 = symbolStack;
    s2 = statusStack;

    cout << "符号栈：";
    //输出符号栈内容
    while (!s1.empty()) {
        s = s1.top();
        s1.pop();
        cout << s << " ";       

    }    
    cout << endl;
    cout << "状态栈：";
    //输出状态栈
    while (!s2.empty()) {
        i = s2.top();
        s2.pop();
        cout << i << " ";
 
    }    
    cout << endl;
}



void SyntaxAnalysis::LRAnalyse(list<Word> words) {
    
   
    Word tempword;
    string vtsymbol;//下一个要接受的符号
    int status;//状态栈栈顶 
    char c;

    statusStack.push(0);//初始状态0压入栈中

    /*set<string> s;
    s = Vn;
    s.insert("id");
    s.insert("num");
    s.insert("character");
    s.insert("int");
    s.insert("char");
    s.insert("bool");
    s.insert("true");
    s.insert("false");

    mySemantic.buildAttributeGrammar(s);*/


    for (auto it = words.begin(); it != words.end(); it++) {
        tempword = *it;
        vtsymbol = tranSymbol(tempword);        
        do {
            status = statusStack.top();            
            c = analyse(status, vtsymbol, tempword);
            cout << endl;
            if (c == 'E')
                break;
        } while (c == 'r');
        if (c == 'E')
            break;
    }
}



vector<string> SyntaxAnalysis::msplit(string st, char sign) {
    vector<string> str;

    for (int i = 0; i < st.length(); i++) {
        string temp = "";
        while (st[i] != sign && i < st.length()) {
            temp += st[i];
            i++;
        }
        str.push_back(temp);
    }
    return str;
    
}



void SyntaxAnalysis::readLR1Table(string path) {
    ifstream file;
    file.open(path, ios::in);

    if (!file.is_open()) {
        cout << "无法打开文件，请重试！" << endl;
    }

    string str;
    int i, line;
    vector<string> v;
    line = 0;
    while (getline(file, str)) {
        v = msplit(str, ' ');
        for (i = 0; i < vt_num; i++)
            ACTION[line][i] = v[i];

        for (i = 0; i < vn_num; i++)
            GOTO[line][i] = atoi(v[i + vt_num].c_str());

        line++;
    }

}





