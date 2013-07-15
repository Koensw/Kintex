#include "env.h"
#include "exception.h"

using namespace kintex;

SymbolTable &Environment::getSym() {
	if(symRef == 0) throw TableException();
	return *symRef; 
}