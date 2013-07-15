#ifndef KINTEX_EXCEPTION_H 
#define KINTEX_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <cctype>
#include <string>

#include <sstream>
#include <typeinfo>

#include "processor.h"
#include "interpreter.h"
#include "env.h"

namespace kintex{
    
    /* Define the ProgramErrors:
     * These errors are critical for the program and should terminate them
     */
    class UnreachableException: public std::exception{
    public:
        UnreachableException() {}
        ~UnreachableException() throw() {}
        
        /* Generate error message */
        const char *what() const throw(){
            return "unreachable method called";
        }
    };
    
    class TableException: public std::exception{
   	public:
    	TableException() {}
    	~TableException() throw() {}
    	
    	/* Generate error message */
        const char *what() const throw(){
            return "symbol table is corrupt (for example missing levels)";
        }
    };
    
    /* Define the ScriptErrors:
     * This are the error, that are caused by error in the script, but before reading syntax with processor or after executing.
     */
    
    // abstract base class script error 
    class ScriptError{
    public:
        ScriptError() {}
        ~ScriptError() throw() {}
        
        /* Derive this to generate error message */
        virtual std::string what() const throw(){
            return "An unknown ScriptError has occurred";
        }
    };
    
    
    /* Define the SyntaxErrors:
     * These are closely related with the Processor so there are friends
     */
    
    //abstract base class
    class SyntaxError{
    public:
        SyntaxError(Processor &p): proc(p) {}
        ~SyntaxError() throw() {}
        
        /* Get string where reading goes wrong */
        virtual std::string getErrorString() const{
			if(isprint(proc.getChar())){
				return std::string(1, proc.getChar());
			}else{
				return std::string("<unknown>");
			}
        }
        virtual std::string getErrorPointerString() const;
        /* Get token string */
        virtual std::string getTokenString() const;
        
        /* Derive this to generate error message */
        virtual std::string what() const throw(){
            return "An unknown SyntaxError has occurred";
        }
    protected:
		/* Get identifier if used */
		std::string getIdentifier() const;
        Processor proc;
    };
    
    //class that handles tokens that are not known by the Processor
    class UnknownToken: public SyntaxError{
    public:
        UnknownToken(Processor &p): SyntaxError(p){}
        std::string what() const throw(){
            std::string ret("Token '");
            ret += getErrorString();
            ret += "' could not be matched by any operator or operand.";
            return ret.c_str();
        }
        
        ~UnknownToken() throw() {}
    private:
        char unknown;
    };
    
    //class that handles tokens that are expected but not there
    class MissingToken: public SyntaxError{
    public:
        MissingToken(Processor &p): SyntaxError(p) {}
        std::string what() const throw(){
            std::string ret("Token is expected but not found.");
            return ret.c_str();
        }
    };
    
    //class that handles tokens that are unbound
    class LeftToken: public SyntaxError{
    public:
        LeftToken(Processor &p, Expression expr): SyntaxError(p), leftExpression(expr) {}
        
        /* Get error pointer string */
        std::string getErrorPointerString() const;
        
        std::string what() const throw(){
            std::ostringstream sstr;
            sstr << "Token '" << leftExpression << "' is left after processing request.";
            return sstr.str().c_str();
        }
        ~LeftToken() throw(){}
    private:
        Expression leftExpression;
    };
    
    //handle invalid syntax that are not thrown by classes above
    class ParseError: public SyntaxError{
    public:
        /* Default constructor */
        ParseError(Processor &p, std::string msg): SyntaxError(p), message(msg) {} 
        
        /* Get error pointer string */
        std::string getErrorPointerString() const{
            //FIXME: return correct string
            return std::string();
        }
        
        std::string what() const throw(){
            return message;
        }
        
        ~ParseError() throw(){}
    private:
        std::string message;
    };
    
    /* 
     * Define the ExecErrors:
     * These are thrown when calling the result() method when an calculation could not be made
     */
    class ExecError{
    public:
        /* Default constructor */
        ExecError(const Token &givenError, Environment ev): error(givenError), env(ev) {}
        ExecError(const Token &givenError): error(givenError) {}
        /* Default pointer (for parents) constructor */
        ExecError(Token *givenError, Environment ev): error(givenError), env(ev) {}
        ExecError(Token *givenError): error(givenError) {}
        
        /* Returns expression */
        virtual std::string getExpressionString() const {
            std::ostringstream sstr;
            sstr << error;
            return sstr.str();
        }
        
        /* Get pointing string: SHOULD BE PRINTED RIGHT UNDER TokenString 
         */
        virtual std::string getErrorPointerString() const;
        
        /* Returns token string
         */
        virtual std::string getTokenString() const;
        
        /* Derive this to generate error message */
        virtual std::string what() const throw(){
            return "A unknown ExecError has occurred.";
        }
        
        ~ExecError() throw() {}
    protected:
		/* Get identifier if used */
		std::string getIdentifier() const;
        Expression error;
		Environment env;
    };
    
    class UnsupportedOperation: public ExecError{
    public:
        /* Get result of children, because after construction this error should be fully constant (no other exception could be thrown) */
        UnsupportedOperation(const Token &child);
		UnsupportedOperation(const Token &child, std::vector<Expression> vals): ExecError(child.getParent()->clone()), children(vals) {}
        
        /* Message thrown */
        std::string what() const throw();
        
        ~UnsupportedOperation() throw(){}
    private:
        std::vector<Expression> children;
        
    };
    
    class UndefinedName: public ExecError{
    public:
        /* Default constructor */
        UndefinedName(const Token &givenVar): ExecError(givenVar) { } 
        
        /* Message thrown */
        std::string what() const throw();
        
        ~UndefinedName() throw(){}
    };
    
    //handle operations that could'nt be made because of an other reason then imcompatible types (like division by zero)
    class CalculationError: public ExecError{
    public:
        /* Default constructor */
        CalculationError(const Token &tok, std::string msg): ExecError(tok), message(msg) {} 
        
        std::string what() const throw(){
            return message;
        }
        
        ~CalculationError() throw(){}
    private:
        std::string message;
    };
}

#endif
