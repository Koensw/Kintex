#ifndef KINTEX_ENV_H
#define KINTEX_ENV_H

#include <string>
#include <map>
#include <list>
#include <vector>
#include <stack>
#include <iterator>
#include <utility>
#include <iostream>
#include <string>
#include <map>

#include "interpreter.h"
#include "table.h"
#include "name.h"

namespace kintex{  
    class Environment{
    public:
		Environment(): symRef(0) {}
    	Environment(SymbolTable &sr): symRef(&sr) {}
    	
    	SymbolTable &getSym();
    	DynamicSymbolTable &getDynSym() {return dynsym;}
    private:
		SymbolTable *symRef;
		DynamicSymbolTable dynsym;
    };
}

#endif
