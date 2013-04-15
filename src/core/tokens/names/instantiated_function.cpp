#include <iostream>
#include <cctype>

#include "names.h"

using namespace kintex;

/* Return result function with argument values set */
Value InstantiatedFunction::result(){
    //set correct parent and line for function
    func->parent = getParent();
    func->line = line;
    //set correct argument for values
    for(auto iter = argumentValues.begin(); iter < argumentValues.end(); ++iter){
        (*iter)->parent = func;
    }
    //execute bound function with argument values if not void
    return (*func)(argumentValues);
}

/* Display function with arguments */
std::ostream &InstantiatedFunction::display(std::ostream &out) const{
    func->display(argumentValues, out);
    return out;
}

/* Returns the contents of the function --> the function that is instantiated */
Token &InstantiatedFunction::getContents(){
    return *func;
}