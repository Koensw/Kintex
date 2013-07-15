#ifndef KINTEX_NAME_H
#define KINTEX_NAME_H

#include <iostream>
#include <string>
#include <typeinfo>

#include "token.h"

namespace kintex{
	class Operand;
	class Environment;

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
        
        /* Returns name */
	    std::string getId() { return name; }
    protected:
        std::string name;
    };
    
    class DynamicToken: public Name {
    public:
		DynamicToken(std::string id): Name(id) {}
		~DynamicToken() {}
		
		//Dynamic tokens could not be created and are not existing for the user
		Token *create(Processor&) { return nullptr; };
		
		/** Redefine pure virtual */
		virtual DynamicToken *clone() const = 0;

        /* Special set-operator for names */
		virtual Operand &set(Operand &op, Environment &env) = 0;
	public:
        Expression val;
    };
    
    class Creator: public Token{
    public:
        /* Default constructor */
        Creator(std::string nm): name(nm) {} 
        ~Creator() {}
   
        /* Result function --> should never be reached! */
        Value result(Environment &env);
        /* Display function --> should never be reached! */
        std::ostream &display(std::ostream &out) const;
        
        /* Get registered name */
		//FIXME: find better way
        std::string getLevelName() {return name;}
    private:
    	std::string name;
    };
}

#endif
