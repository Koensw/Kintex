#ifndef KINTEX_NAME_H
#define KINTEX_NAME_H

#include <iostream>
#include <string>
#include <typeinfo>

#include "token.h"

namespace kintex{
	class Operand;

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
    
    class DynamicToken: public Name {
    public:
    	enum Type{
    		EXTEND, SINGLE
    	};
    
		DynamicToken(std::string id, Type tp): Name(id), type(tp) {}
		~DynamicToken();
		
		//Dynamic tokens could not be created and are not existing for the user
		Token *create(Processor&) { return nullptr; };
		std::ostream &display(std::ostream&) const;
		std::string getName() const;
		Token &getContents();
		
		/** Redefine pure virtual clone to give an DynamicToken back --> else DynamicExpression class is not working */
		virtual DynamicToken *clone() const = 0;
		
	    std::string getId() { return name; }
	protected:
		Type type;
    };
    
    class Creator: public Token{
    public:
        /* Default constructor */
        Creator(std::string nm): name(nm) {} 
        ~Creator() {}
   
        /* Result function --> should never be reached! */
        Value result();
        /* Display function --> should never be reached! */
        std::ostream &display(std::ostream &out) const;
        
        /* Get registered name */
        std::string getLevelName() {return name;}
    private:
    	std::string name;
    };
}

#endif
