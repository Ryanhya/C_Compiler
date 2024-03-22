#include <string>
#include "Term.h"



using namespace std;



bool Term::termEqual(Term t) {
	if (t.pleft == this->pleft && t.pright == this->pright && t.forward == this->forward && t.dot == this->dot)
		return true;
	else
		return false;		
}

