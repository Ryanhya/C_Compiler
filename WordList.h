#pragma once
#include <list>
#include "Word.h"

using namespace std;


class WordList {
public:
	list<Word> wordList;
	void writeWordListFile(string path);
};

