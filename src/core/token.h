//define tokens

#ifndef KINTEX_TOKEN_H
#define KINTEX_TOKEN_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <utility>
#include <memory>

#include "interpreter.h"

namespace kintex{

    class Operand;
    class Processor;
    class Level;

    class Token{
    	friend class Statement;
        friend class Processor;
        friend class ParenthesesOperator;
        friend class BracketsOperator;
        friend class InstantiatedFunction;
        friend class ExecError;
        friend class LeftToken;
        friend class UnsupportedOperation;
        public:
            /* Default constructor: explicit defined because of copy-constructor */
            Token(): parent(nullptr) {}
            
            /** Returns result of expression
            */
            virtual Value result() = 0;
            /** Display token value to stream
            */
            virtual std::ostream &display(std::ostream&) const = 0;
            /** Create a copy of token, for use within Expression (copy constructor)
            */
            virtual Token *clone() const = 0;
            /** Create a new token from Processor
             * - return a pointer to itself if token can be created
			 * - return itself if name
             * - else return nullptr
            */
            virtual Token *create(Processor&) = 0;
            
            /* Special functions that should be redeclared when other behaviour is expected */
            /** Return the level index where children should be added 
             * - return zero level (default) to not add to a level (FIXME: no special zero-level)
             * (normally used only in creators)
             */
            virtual int getAddLevelIndex() const { return 0; }
            /** Return the contents (child) of a token, if the token itself is not really part of the expression
             * (normally used in parentheses and brackets)
             */
            virtual Token &getContents() { return *this; }
            /** Returns the name of the class
             * - for operators this should be the name of the operation (like addition)
             * - for operands this should be the name of the operand (like integer)
             * - for parentheses/brackets this should be the name of the child
             * - for functions/variables this should be their names
             */
            virtual std::string getName() const { return "Token";}
            
            /* Get parent */
            Token *getParent() const;

            virtual ~Token(){}
        protected:
            /* Line and positions */
            Line line;
            
            /* Child expressions */
            std::vector<Expression> children;
            /* Parent expression
            WARNING: when used in exception: use only in initialization, because the pointer could else already been freed!
            */
            Token *parent;
    };
}

#endif
