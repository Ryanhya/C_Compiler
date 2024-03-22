#include <list>
#include <string>
#include <fstream>
#include "SymbolList.h"

using namespace std;

void SymbolList::writeSymbolListFile(string path) {
    ofstream file;
    file.open(path, ios::out);
    list<SymbolItem>::iterator plist;
    string str;
    file << "name " << "type " << "attribute " << "storageType " << "addr " << "level " << "line " << endl;
    for (plist = symbolList.begin(); plist != symbolList.end(); plist++) {
        str = (*plist).name + "     " + to_string((*plist).type) + "       " + to_string((*plist).attribute) + "       " + to_string((*plist).storageType) + "      " + to_string((*plist).addr) + "     " + to_string((*plist).level) + "     " + to_string((*plist).line);
        file << str << endl;
    }
    file.close();
}
