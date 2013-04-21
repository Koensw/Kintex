#include <typeinfo>

#include "operators.h"
#include "../names/names.h"
#include "../../processor.h"
#include "../../exception.h"
#include "../../token.h"

using namespace kintex;

SetOperator *SetOperator::create(Processor &p){
    //check if match:
    //no: return NULL
    if(p.getChar() != op[0]) return nullptr;
	Position pos = p.getPos();
	
    //yes: goto next char and create the operator
    p.nextChar();
    return new SetOperator(p.getPrevExpression(pos), p.getNextExpression(pos));
}

//FIXME: support reassign of other type of vars
Value SetOperator::result(){
    //check if ret is a variable or function -> else throw UnsupportedOperation
    if(typeid(children[0]->getContents()) == typeid(Variable)){
        //cast token to name (always succeed, because it is a variable)
        Name &ret(dynamic_cast<Name&>(children[0]->getContents()));
        
        ret = *(children[1]->result());
        return ret.result();
    }else if(typeid(children[0]->getContents()) == typeid(Function)){
        //cast token to function
        Function &ret(dynamic_cast<Function&>(children[0]->getContents()));
        
        //check if redefinition (not first definition)
        if(typeid(*children[0]) == typeid(InstantiatedFunction)){
            //cast children to instance
            InstantiatedFunction &instance(dynamic_cast<InstantiatedFunction&>(*children[0]));
        
            //set new parameters if possible (parameters should be variables, else throw exception)
            for(size_t i=0;i<ret.arguments.size();++i){
                if(typeid(instance.argumentValues[i]->getContents()) != typeid(Variable)) throw UnsupportedOperation(*children[0]);
                ret.arguments[i] = instance.argumentValues[i];
            }
        }
        ret.expr = children[1];
        
        return Void(this);
    }else throw UnsupportedOperation(*children[0]);
}
