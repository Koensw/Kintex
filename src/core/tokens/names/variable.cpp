#include <iostream>
#include <typeinfo>
#include <cctype>

#include "names.h"
#include "../../processor.h"

using namespace kintex;

/* Make a new variable from a string */
Variable *Variable::create(Processor &p){
    //check if variable is same
    if(p.getChar() == name[0]){
        unsigned int i = 1;
        for(;i<name.length();++i){
            if (p.lookNextChar(i) != name[i]) return nullptr;
        }
        
        //check if finished
        if (i==name.length()){
            //check if not more alpha characters
            if(isalpha(p.lookNextChar(i))) return nullptr;
            
            //match --> return itself
            while(i--!=0) p.nextChar();
            return this;
        }
    }
    
    //if no match found return nullptr
    return nullptr;
}

/* Special set-operator to set new values to the variable */
Operand &Variable::operator=(Operand &op){
    //check if not same type
    if(typeid(op) == typeid(Void)){
        //this is not a supported operation
        throw UnsupportedOperation(op);
    }
    else if(typeid(*val) != typeid(op)){
        //cast value to type of operand (weak type!) and set it to value
        val = op.clone();
    }else{
        //just set the value to the operand
        *val = op; 
    }
    
    //return value (but not direct, to protect it)
    return op;
}