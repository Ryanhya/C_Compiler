#pragma once
#include <list>
#include <string>
#include "SymbolItem.h"

using namespace std;


class SymbolList {
public:
	list<SymbolItem> symbolList;
	void writeSymbolListFile(string path);
};