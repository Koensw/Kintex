#include <iostream>
#include <cctype>

#include "names.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new variable */
Variable *VariableCreator::create(Processor &p){
    //create variable
    std::string varName;
    if(isalpha(p.getChar())){
        varName += p.getChar();
        
        //loop until space or end of expression
        int i = 1;
        while(isalpha(p.lookNextChar(i))){
            varName += p.lookNextChar(i);
            ++i;
        }
        
        //skip tokens
        while(i--!=0) p.nextChar();
        
        //register token
        p.registerGlobal(getLevelName());
        
        //built token
        Variable *var = new Variable(varName); 
        return var;
    }
    
    //if creation was not succesfull return nullptr
    return nullptr;
}
