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
#include "token.h"

namespace kintex{  
    class Environment{
    public:
    	Environment() {}
    	
    	//get 'best' token in current environment
    	Expression getToken(std::string id);
    	Expression isToken(std::string id);
    	
    	//add token to dynamic environment
    	void addToken(DynamicToken *tok);
    	void removeToken(std::string id);
    	
    	//extend/reduce the score
    	void extend();
    	void reduce();
    private:
    	 dyntable;
    }
}

#endif
