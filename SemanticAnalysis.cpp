#include <string>
#include "SemanticAnalysis.h"




using namespace std;


void SemanticAnalysis::getSymbolListIndex(list<SymbolItem> slist) {
	int i = 0;
	for (auto it = slist.begin(); it != slist.end(); it++) {
		toSymbolList[i] = it;
		i++;
	}
}




int SemanticAnalysis::entry(string name) {	
	list<SymbolItem>::iterator tmp;
	int i;

	for (i = 0; i < symbol_num; i++) {
		tmp = toSymbolList[i];
		if ((*tmp).name == name)
			return i;
	}
	
	
}



int SemanticAnalysis::getAddr(string name) {
	int i;
	list<SymbolItem>::iterator tmp;

	for (i = 0; i < symbol_num; i++) {
		tmp = toSymbolList[i];
		if ((*tmp).name == name) {
			return (*tmp).addr;
		}
	}

}


void SemanticAnalysis::addAddr(int index, int address) {	
	(*toSymbolList[index]).addr = address;
}

void SemanticAnalysis::addType(int index, int type) {
	(*toSymbolList[index]).type = type;
}




void SemanticAnalysis::Gen(string op, int arg1, int arg2, int result) {
	Code c;

	c.op = op;
	c.arg1 = arg1;
	c.arg2 = arg2;
	c.result = result;

	codes[nextquad] = c;
	nextquad++;


}



int SemanticAnalysis::newtemp() {
	return mid_index;
}


vector<int> SemanticAnalysis::makelist(int quad) {
	vector<int> v;
	v.push_back(quad);
	return v;
}


vector<int> SemanticAnalysis::merge(vector<int> v1, vector<int> v2) {
	vector<int> v;
	int i, j;
	bool flag = false;

	for (i = 0; i < v1.size(); i++) {
		v.push_back(v1[i]);
	}

	for (i = 0; i < v2.size(); i++) {
		//判断是否已经存在
		for (j = 0; j < v1.size(); j++) {
			if (v2[i] == v1[j]) {
				flag = true;
				break;
			}

		}
		if (flag)
			continue;
		v.push_back(v2[i]);

	}

	return v;

}


void SemanticAnalysis::backpatch(vector<int> p, int quad) {
	int i, index;
	for (i = 0; i < p.size(); i++) {
		index = p[i];
		codes[index].result = quad;
	}
}





void SemanticAnalysis::translate(Production p) {
	string pleft, pright;
	vector<string> right;
	int i;

	pleft = p.pleft;
	pright = p.pright;
	right = p.right;

	if (pleft == "Y") {
		if (pright == "OY") {
			
		}
	}
	else if (pleft == "O") {
		if (pright == "C") {
			
		}

	}



}

