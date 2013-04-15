#ifndef KINTEX_NAME_H
#define KINTEX_NAME_H

#include <iostream>
#include <typeinfo>

#include "token.h"
#include "exception.h"
#include "operand.h"

namespace kintex{
    class Name: public Token{
        friend class UndefinedName;
    public:
        /* Default constructor */
        Name(std::string givenName): name(givenName) {}
       
        /* Display function */
        std::ostream &display(std::ostream &out) const{
            out << name;
            return out;
        }
        
        /* Special set-operator for names */
        virtual Operand &operator=(Operand &op) = 0;
    protected:
        std::string name;
    };
    
    class Creator: public Token{
    public:
        /* Default constructor */
        Creator(int index): addLevelIndex(index) {} 
        /* Redefine add level (special for creators) */
        int getAddLevelIndex() const { return addLevelIndex; }
        
        /* Result function --> should never be reached! */
        Value result(){
            throw UnreachableException();
        }
        /* Display function --> should never be reached! */
        std::ostream &display(std::ostream &out) const{
            throw UnreachableException();
        }
    private:
        int addLevelIndex;
    };
}
#endif