#ifndef KINTEX_NAMES_H
#define KINTEX_NAMES_H

#include <iostream>
#include <sstream>
#include <typeinfo>

#include "../../name.h"
#include "../../operand.h"
#include "../operands/operands.h"
#include "../../interpreter.h"
#include "../../exception.h"
#include "../../env.h"

namespace kintex{
    /*
     * Variable classes 
     */
    
    class Variable: public DynamicToken{
		friend class DynamicToken;
    public:
        /* Default constructor */
        Variable(std::string name): DynamicToken(name) {}
        /* Clone function */
        Variable *clone() const {return new Variable(*this);}
        /* Result function --> return clone of bound value */
        Value result(Environment &env);
        /* Name function */
        std::string getName() const{ return "variable";}
        
        /* Special set operator */
        Operand &set(Operand &op, Environment &env);
    };
    
    /* Creator for variables */
    class VariableCreator: public Creator{
    public:
        /* Default constructor */
        VariableCreator(std::string nm): Creator(nm) {}
        /* Clone function --> normally not needed */
        VariableCreator *clone() const {return new VariableCreator(*this);}
        /* Create operation -> returns a variable */
        Variable *create(Processor&);
    };
    
    /*
     * Function classes
     */
    
    /* Instance of a function */
    class FunctionBase;
    class InstantiatedFunction: public Token{
        friend class SetOperator;
    public:
        /* Default constructor */
        InstantiatedFunction(FunctionBase *givenFunc, std::vector<Expression> argVals): func(givenFunc), argumentValues(argVals) {}
        /* Clone function --> function clone is actually the same! */
        InstantiatedFunction *clone() const {return new InstantiatedFunction(*this);}
        /* Result function --> return result function */
        Value result(Environment &env);
        /* Display function */
        std::ostream &display(std::ostream &out) const;
        /* Create operation --> should never be reached! */
        InstantiatedFunction *create(Processor&){
            throw UnreachableException();
        }
        /* Name function */
        std::string getName() const{ return "function instance";}
        /* Get contents returns the associated function */
        Token &getContents();
    private:
        FunctionBase *func;
        std::vector<Expression> argumentValues;
    };
    
    /* BASE FUNCTION
     * All childs should implement following methods
     * - (empty) constructor calling the FunctionBase constructor with the name and the size of the arguments
     * - clone method returning copy of itself
     * - operator()(std::vector<Expression> values) method that does the functions work
     */
    class FunctionBase: public Name{
        friend class InstantiatedFunction;
    public:
        /* Default constructor */
        FunctionBase(std::string givenName, size_t argSize): Name(givenName), argumentSize(argSize){}
        /* Result function */
        Value result(Environment &env){throw UndefinedName(*this);}
        /* Create operation */
        InstantiatedFunction *create(Processor&);
        /* Name function */
        std::string getName() const{ return "function";}
        
        /* Special operator that executes the function */
        virtual Value operator()(std::vector<Expression> values, Environment &env) = 0;
        /* Special display operator for function */
        virtual std::ostream &display(std::vector<Expression> values, std::ostream &out) const{
            out << name;
            out << "(";
            std::vector<Expression>::const_iterator valIter = values.begin();
            for(;valIter < values.end(); ++valIter){
                out << *valIter;
                if(valIter+1 != values.end()) out << ",";
            }
            out << ")";
            return out;
        }
    protected:
        size_t argumentSize;
    };
    
    /* Function itself */
    class Function: public FunctionBase{
        friend class SetOperator;
    public:
        /* Default constructor */
        Function(std::string givenName, std::vector<Expression> args): FunctionBase(givenName, args.size()), expr(Void()), arguments(args) {}
        /* Clone function */
        Function *clone() const{return new Function(*this);}
        /* Special result function */
        Value operator()(std::vector<Expression> values, Environment &env){
            //if function not defined return error
            if(typeid(*expr) == typeid(Void)) throw UndefinedName(*this);
            //set arguments to corresponding value
            std::vector<Expression>::iterator argIter = arguments.begin();
            std::vector<Expression>::iterator argValIter = values.begin();
			//increase scope
			env.getDynSym().extend(DynamicSymbolTable::FUNCTION);
            for(;argIter < arguments.end(); ++argIter, ++argValIter){
                //cast token to name (always succeed, because it is a variable)
				Variable &op(dynamic_cast<Variable&>((**argIter).getContents()));
                
                //evaluate expression and assign result to vars 
                op.set(*(*argValIter)->result(env), env);
            }
            
            //return result expression
            Value ret = expr->result(env);
			//decrease scope
			env.getDynSym().reduce();
			return ret;
        }
    private:
        Expression expr;
        std::vector<Expression> arguments;  
    };
    
    
    /* Creator for variables */
    class FunctionCreator: public Creator{
    public:
        /* Default constructor */
        FunctionCreator(std::string nm): Creator(nm) {}
        /* Clone function --> normally not needed */
        FunctionCreator *clone() const{return new FunctionCreator(*this);}
        /* Create operation -> returns a variable */
        Function *create(Processor&);
    };
    
}
#endif
