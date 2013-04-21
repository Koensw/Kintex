#include <iostream>
#include <cctype>

#include "names.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new function */
InstantiatedFunction *FunctionBase::create(Processor &p){
	//check if correct
	if(p.lookString(name.size()) == name && !isalpha(p.lookNextChar(name.size()))){
		Position pos = p.getPos();
		p.nextChar(name.size());
		
		//get argument values
		std::deque<Expression> argVals;
		for(size_t i=0; i<argumentSize; ++i){
			Expression expr = p.getNextExpression(pos);
			argVals.push_back(expr);
		}
		
		//make instance
		InstantiatedFunction *instance = new InstantiatedFunction(this, std::vector<Expression>(argVals.begin(), argVals.end()));
        return instance;
	}
	
	return nullptr;
}