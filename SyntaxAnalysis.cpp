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

//��ȡ�ķ��е��ս����
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
    
   
    //����ƥ��˫�ַ�����ֹҪƥ��>=ʱƥ�䵽>���˳�
    for (k = 3; k < 27; k++)
    { //����Ӧ��ע�⣺ʹ���ƥ��
        
        if (production.substr(0, 2) == Vt[k])
            return production.substr(0, 2);
    }

    for (k = 3; k < 27; k++)
    {
        if (production.substr(0, 1) == Vt[k])
            return Vt[k];
    }
   

    //ƥ��ؼ���
    string tmp;
    for (k = 27; k < vt_num; k++)
    {
        tmp = production.substr(0, Vt[k].length());
        if (tmp == Vt[k])
        {
            return tmp;
        }
    }
    //������е����˵������ս�������ս���ű���ģ���ΪСд��ĸҲ�����ս��
    /*if (production[0] >= 'a' && production[0] <= 'z')
    {
        return production.substr(0, 1);
    }*/
    
}


//��һ������ʽ���Ҳ����ֳɷ��ս���ź��ս���ŵ����
vector<string> SyntaxAnalysis::splitRight(string right) {
    
    vector<string> v;
    int j;
    int length = right.length();

    for (j = 0; j < length; j++)
    {
        //�����ո�
        if (right[j] == ' ')
        {
            continue;
        }
        if (right[j] >= 'A' && right[j] <= 'Z')
        { //���ս��
            string Vn="";
            if ((j == length - 1) || (j + 1 < length && right[j + 1] != '\''))//����������S'�ķ��ս����
                Vn = Vn + right[j];
            else 
                Vn = Vn + right[j] + '\'';
            v.push_back(Vn);
            j = j + Vn.length() - 1;
            Vn.clear();
        }
        else
        { //�ս��
            string vt;
            vt = getVt(right.substr(j, length - j));
            v.push_back(vt);
            j = j + vt.length() - 1;
            vt.clear();
        }
    }
    return v;
   
}

//��ȡ�ķ�����ʽ
void SyntaxAnalysis::readGrammarFile(string path) {
	ifstream file;
    file.open(path, ios_base::in);
    if (!file.is_open()) {
        cout << "�޷����ļ��������ԣ�" << endl;
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
            //���ļ�ʱ����Դ������ս���ż�Vn
            Vn.insert(p.pleft);
        }
        //�������'�ķ��ս����
        else {
            tmp = SyntaxAnalysis::splitRight(str.substr(4, str.length() - 4));
            p.pleft = str.substr(0,2);
            p.pright = str.substr(4, str.length() - 4);
            p.right = tmp;
            grammar.push_back(p);
            //���ļ�ʱ����Դ������ս���ż�Vn
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


//�ݹ��󵥸�symbol��First��
set<string> SyntaxAnalysis::getSymbolFirst(string symbol) {
    set<string> tmp;
    //bool emptyFlag = false;//symbol�Ƿ����Ƶ�Ϊ��
    //�ս���ŵ�First��Ϊ������
    if (isVt(symbol)) {
        tmp.insert(symbol);
        return tmp;
    }
    else {//symbolΪ���ս����
        int i;
        //��������ʽ����
        for (i = 0; i < grammar.size(); i++) {
            //��Ϊsymbol�Ĳ���ʽ
            if (grammar[i].pleft == symbol) {
                //��Ϊ��ݹ��������ò���ʽ
                if (grammar[i].pleft == grammar[i].right[0])
                    continue;
                string fir_right = grammar[i].right[0];
                //����ʽ�Ҳ���һ������Ϊ�ս����
                if (isVt(fir_right)) {
                    tmp.insert(fir_right);//����First����
                    //��symbol���Ƶ�Ϊ��
                    //if (emptyFlag) {
                    //    //tmp.
                    //}

                }
                else {
                    //set<int> s;
                    set<string> next;
                    //s = emptyIndex();
                    //����fir_right�ܷ��Ƶ�Ϊ��
                    //for (auto it = s.begin(); it != s.end(); it++) {
                        //if (grammar[*it].pleft == fir_right) {
                            //emptyFlag = true;
                            //break;
                        //}
                    //}
                    next = getSymbolFirst(fir_right);
                    tmp.insert(next.begin(), next.end());//��tmp��next�ϲ�
                }
            }
        }
        return tmp;
    }
}




void SyntaxAnalysis::getFirst() {
    set<string> s;
    int i, j;
    //�Ƚ����ս���ŵ�First������Ϊ������
    for (i = 0; i < vt_num; i++) {
        s.insert(Vt[i]);
        allFirst[Vt[i]].insert(s.begin(), s.end());
        s.clear();
    }
    for (i = 0; i < grammar.size(); i++) {
        for (j = 0; j < grammar[i].right.size(); j++) {
            s = getSymbolFirst(grammar[i].right[j]);
            //sֻ������ʱ�������ʽ�Ҳ�����һ�����ŵ�First��
            if (!(s.size() == 1 && s.count("#") == 1))
                break;
        }
        allFirst[grammar[i].pleft].insert(s.begin(), s.end());
        allFirst[grammar[i].pright].insert(s.begin(), s.end());//set�ϲ�
    }
}

//����ս����symbol��Follow��
set<string> SyntaxAnalysis::getSymbolFollow(string symbol) {
    set<string> s;
    int i;
    //��symbolΪ��ʼ���ţ�������$������Follow��
    if (symbol == start_symbol) {
        s.insert("$");
        return s;
    }


    //��������ʽ����
    for (i = 0; i < grammar.size(); i++) {
        //symbol���ڵ�i������ʽ���Ҳ�������һ������ʽ��Ѱ��
        if (grammar[i].pright.find(symbol) == grammar[i].pright.npos)
            continue;
        else {
            int j, index, k;
            string next;

            for (j = 0; j < grammar[i].right.size(); j++) {
                if (grammar[i].right[j] == symbol) {
                    index = j + 1;//��һ�����ŵ�����
                    set<string> tmp;
                    //ǡ��Ϊ�Ҳ����һ������
                    if (index == grammar[i].right.size()) {
                        if (grammar[i].pleft == symbol)
                            continue;//����������ʽ��ǡ��Ϊsymbol��FOLLOW(symbol)����FOLLOW(symbol)�����壬�Ҹ�������ʽ�Ҳ�����һ������
                        //tmp = getSymbolFollow(grammar[i].pleft);//�����ʽ�󲿵�FOLLOW��(����ջ���������
                        tmp = allFollow[grammar[i].pleft];//Ҫ���󲿵�FOLLOW������symbol��FOLLOW��ǰ�Ѿ����
                        s.insert(tmp.begin(), tmp.end());
                    }
                    else {
                        for (k = index; k < grammar[i].right.size(); k++) {
                            next = grammar[i].right[k];
                            tmp = allFirst[next];//����First��
                            if (!tmp.count("#")) {
                                s.insert(tmp.begin(), tmp.end());
                                break;//�����������ֱ�������Ժ���First�������
                            }
                            else {
                                tmp.erase("#");
                                s.insert(tmp.begin(), tmp.end());
                            }
                            if (k == grammar[i].right.size() - 1) {//ָ��ָ���Ҳ����һ��������֮ǰ��δ�˳���ѭ����
                                if (grammar[i].pleft == symbol)
                                    break;//����������ʽ��ǡ��Ϊsymbol��FOLLOW(symbol)����FOLLOW(symbol)�����壬�Ҹ�������ʽ�Ҳ�����һ������
                                //tmp = getSymbolFollow(grammar[i].pleft);//�����ʽ�󲿵�FOLLOW��(����ջ���������
                                tmp = allFollow[grammar[i].pleft];//Ҫ���󲿵�FOLLOW������symbol��FOLLOW�����Ѿ����                              
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
    //�������в���ʽ�󲿵�Follow��,��Ϊ���ս����һ������
    for (i = 0; i < grammar.size(); i++) {
        s = getSymbolFollow(grammar[i].pleft);
        allFollow[grammar[i].pleft].insert(s.begin(), s.end());
    }
    //���⴦����ڵĻ�·
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
    int nextIndex = t.dot + 1;//��һ��Ҫʶ��ķ��ŵ�����
    //������һ������
    if (nextIndex < t.right.size()) {
        //�ս����
        if (isVt(t.right[nextIndex]) && t.right[nextIndex] != "#") {
            return 1;
        }
        else if(t.right[nextIndex] == "#"){
            return 2;
        }
        else {
            return 3;//��Լ��Ŀ
        }
    }
    else {
        return 2;//Ϊ��Լ��Ŀ
    }
}


//��ʼ��һ��LR1��ĿΪ�������ĿIno��Term1��Term2...
void SyntaxAnalysis::initialTermCollection(int no, vector<Term> v) {
    int i;
    for (i = 0; i < v.size(); i++) {
        v[i].type = setTermType(v[i]);//����������Ϣ
        v[i].is_core = true;//��עΪ������Ŀ
        LR1_collection[no].push_back(v[i]);
        flags.insert(pair<int, bool>(no, false));//��״̬δ����
        
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


//�����Լ��Ŀterm�ıհ�
vector<Term> SyntaxAnalysis::closure(Term term) {
    vector<Term> s;
    int nextSymbolIndex = term.dot + 1;   
    string nextSymbol = term.right[nextSymbolIndex];

    Term tmp;
    int i;
    string str;
    set<string> first;
    set<string> forward_set;//��ǰ����������
    
    //nextSymbolΪterm���Ҳ����һ������
    if (nextSymbolIndex + 1 == term.right.size()) {
        forward_set.insert(term.forward);
    }
    else {
        for (i = nextSymbolIndex + 1; i < term.right.size(); i++) {
            str = term.right[i];
            first = allFirst[str];
            forward_set.insert(first.begin(), first.end());
            //��ǰ����str��First��������#������ǰ�������������
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

    //��������ʽ����Ѱ����ΪnextSymbol�Ĳ���ʽ
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
    v = LR1_collection[no];//��ȡ������Ŀ
    int i, j;   
    Term t;
    queue<Term> q;//��Լ��Ŀ����
    vector<Term> temp;//�洢������Ŀ��հ����õ�������Ŀ
    
    
    for (i = 0; i < v.size(); i++) {
        //��Ϊ��Լ��Ŀ���ƽ���Ŀ����������
        if (v[i].type != 3)
            continue;
        q.push(v[i]);                      
    }

    //BFS��ÿһ��Ĵ�Լ��Ŀ
    while (!q.empty()) {
        t = q.front();//ȡ����
        q.pop();//����
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
    flags[no] = true;//�������

}

bool SyntaxAnalysis::isCoreTermExist(vector<Term> v) {
    int i;
    int nowIndex;
    bool flag = false;
    vector<Term> collection;//��ǰ״̬�ĺ�����Ŀ
    vector<Term> nowCollection;//��ǰ״̬��ȫ����Ŀ
    //������ǰ���е�״̬
    for (auto it = LR1_collection.begin(); it != LR1_collection.end(); it++) {
        nowIndex = (*it).first;
        nowCollection = (*it).second;
        //��״̬�ĺ�����Ŀ����collection  
        if (flags[nowIndex]) {
            for (i = 0; i < nowCollection.size(); i++) {
                //��Ϊ������Ŀ
                if (nowCollection[i].is_core)
                    collection.push_back(nowCollection[i]);
            }
        }
        else
            collection = nowCollection;
        

        if (collection.size() >= v.size()) {
            for (i = 0; i < v.size(); i++) {
                //��һ����ڣ�����Ŀ����Ȼ��һ��������һ�����е�״̬
                if (!isTermExist(v[i], collection))
                    break;
            }
            if (i == v.size()) {//֮ǰ��δ��������˵��v��collection��ȫƥ���ϣ�v�Ѵ���
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
        //����һ��Ϊfalse��ֱ�ӷ���false
        if (!myflag)
            return false;
    }
    return true;
}



void SyntaxAnalysis::getNewCollection(int no) {
    unordered_map<string, vector<Term>> new_collections;//�ӵ�ǰ״̬no����string�󵽴���״̬�ĺ�����Ŀ
    vector<Term> v = LR1_collection[no];//���״̬no�����������ȫ����Ŀ
    int i;
    int index;
    string nextSym;
    vector<Term> tmp;
    Reflection r;

    for (i = 0; i < v.size(); i++) {        
        if (v[i].type == 2)//��Լ��Ŀ�����ƶ���ǰ׺
            continue;       
        else {//��Լ���ƽ���Ŀ
            index = v[i].dot;
            nextSym = v[i].right[index + 1];//��ǰ׺Ҫʶ�����һ������
            v[i].dot = index + 1;//��ǰ׺�ƽ�һλ
            new_collections[nextSym].push_back(v[i]);            
        }
        
    }

    for (auto it = new_collections.begin(); it != new_collections.end(); it++) {
        nextSym = (*it).first;
        tmp = (*it).second;
        //������Ŀ�Ѿ����ֹ�
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


//���ݿ�ʼ���Ų���ʽ��LR��Ŀ������LR(1)��Ŀ�淶��
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
    //д��״̬���ӳ���ϵ
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
    return -1;//�����ڸ��ս����
}


int SyntaxAnalysis::vn2int(string s) {
    int i;
    for (i = 0; i < vn_num; i++) {
        if (myVn[i] == s)
            return i;
    }
    return -1;//�����ڸ÷��ս����

}

//���й�Լʱ��ȡ����ʽ�±�
int SyntaxAnalysis::getProductionIndex(Term t) {
    int i;
    for (i = 0; i < grammar.size(); i++) {
        if (grammar[i].pleft == t.pleft && grammar[i].pright == t.pright)
            return i;
    }
    return -1;//δ�ҵ�
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
                    k = vt2int(r.received_symbol);//������Vt���±�
                    ACTION[i][k] = "S" + to_string(r.end);//�ƽ�
                }
                else {
                    k = vn2int(r.received_symbol);//������myVn���±�
                    GOTO[i][k] = r.end;
                }
            }
            else {
                break;//reachÿһ���startֵ��С�������У���������ͬ��˵����start����i������㵽��startֵʱ��ѭ��
            }
            j++;

        }


        //��д��Լ��Ŀ����Լ��Ŀ�����Ƴ���#��һ����״̬�ĺ�����Ŀ�У���һ��״̬�ĺ�����Ŀ��������Ŀ����ǰ��
        for (m = 0; m < v.size(); m++) {
            ////�����Ƴ���
            //if (v[m].pright == "#") {
            //    index = getProductionIndex(v[m]);
            //    k = vt2int(v[m].forward);//������Vt�е��±�
            //    ACTION[i][k]= "r" + to_string(index);//�ղ���ʽ��Լ
            //    continue;
            //}
            //if (!v[m].is_core)//���Ǻ�����Ŀ��˵���Ѿ���ѯ����״̬���еĺ�����Ŀ
            //    break;
            if (v[m].type == 2) {
                //��ѯ��Ӧ�Ĳ���ʽ����
                index = getProductionIndex(v[m]);
                k = vt2int(v[m].forward);//������Vt�е��±�
                if (v[m].pleft=="S\'") {
                    ACTION[i][k] = "acc";
                }
                else
                    ACTION[i][k] = "r" + to_string(index);//��Լ
            }
        }

        //δ���ĵط�Ϊ����
        for (m = 0; m < vt_num; m++) {
            if (ACTION[i][m].empty())
                ACTION[i][m] = "ER";//����
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
    //��ʶ��
    if (w.type == 1)
        symbol = "id";
    //���ֳ���
    else if (w.type == 2)
        symbol = "num";
    //�ַ�����
    else if (w.type == 3)
        symbol = "character";
    //�ؼ��֡���������ֽ����
    else
        symbol = w.value;

    return symbol;
}



void SyntaxAnalysis::error(Word w) {
    if (w.value != "$")
        cout << "��" << w.line << "�У���" << w.column << "�г����﷨��������Դ���룡" << endl;
    else
        cout << "Դ�����β����" << endl;
}



char SyntaxAnalysis::analyse(int status, string symbol, Word now_word) {
    int index;//symbol��Ӧ������
    index = vt2int(symbol);
    string action;
    action = ACTION[status][index];

    int number;//ACTION���Ԫ�ص�����
    

    if (action == "ER") {
        //����
        //cout << "��������" << endl;
        showStack();
        error(now_word);
        return 'E';
    }
    //�ƽ�
    else if (action.find("S") != string::npos) {
        number = atoi(action.substr(1).c_str());//�ƽ���״̬
        statusStack.push(number);
        symbolStack.push(symbol);        
        cout << "�ƽ�" << action << endl;
        showStack();
        return 'S';
    }
    //��Լ
    else if (action.find("r") != string::npos) {
        int go;//GOTO���ж�Ӧ��״̬
        int vnindex;//����ʽ�󲿷��ŵ�����
        int l;//����ʽ�Ҳ��ĳ���
        int tmp;//��ǰջ��״̬
        number = atoi(action.substr(1).c_str());//����ʽ������
        if (grammar[number].pright != "#")
            l = grammar[number].right.size();
        else
            l = 0;
        for (int i = 0; i < l; i++) {
            statusStack.pop();
            symbolStack.pop();
        }
        symbolStack.push(grammar[number].pleft);//����ʽ��ѹ�����ջ��
        vnindex = vn2int(grammar[number].pleft);
        tmp = statusStack.top();
        go = GOTO[tmp][vnindex];
        statusStack.push(go);//ѹ��״̬ջ

        showStack();
        cout << "�¸�Ҫ���ܵķ��ţ�" << symbol << endl;
        cout << "ʹ�õĲ���ʽΪ��" << grammar[number].pleft << "->" << grammar[number].pright << "  ";
        cout << "��״̬" << tmp << "ʱ���" << grammar[number].pleft << "ת����״̬" << go << "  ";
        cout << "��Լ" << action << endl;
        return 'r';
    }
    //�����ɹ����˳�
    else if (action == "acc") {
        showStack();
        cout << "����acc���﷨�����ɹ���" << endl;
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

    cout << "����ջ��";
    //�������ջ����
    while (!s1.empty()) {
        s = s1.top();
        s1.pop();
        cout << s << " ";       

    }    
    cout << endl;
    cout << "״̬ջ��";
    //���״̬ջ
    while (!s2.empty()) {
        i = s2.top();
        s2.pop();
        cout << i << " ";
 
    }    
    cout << endl;
}



void SyntaxAnalysis::LRAnalyse(list<Word> words) {
    
   
    Word tempword;
    string vtsymbol;//��һ��Ҫ���ܵķ���
    int status;//״̬ջջ�� 
    char c;

    statusStack.push(0);//��ʼ״̬0ѹ��ջ��

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
        cout << "�޷����ļ��������ԣ�" << endl;
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





