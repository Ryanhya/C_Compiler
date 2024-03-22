#include <list>
#include <string>
#include <fstream>
#include "WordList.h"
#include <iostream>

using namespace std;

void WordList::writeWordListFile(string path) {
    ofstream file;
    file.open(path, ios::out);
    list<Word>::iterator plist;
    string str;
    
    for (plist = wordList.begin(); plist != wordList.end(); plist++) {
        str = "< " + to_string((*plist).type) + " ,  " + (*plist).value + " >";
        file << str << endl;        
    }   
    file.close();
}