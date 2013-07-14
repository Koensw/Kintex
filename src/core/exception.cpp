#include <typeinfo>

#include "exception.h"
#include "processor.h"
#include "interpreter.h"
#include "operand.h"
#include "name.h"

using namespace kintex;

/* Syntax error*/
std::string SyntaxError::getTokenString() const{
    std::string ret;
    //check if a file is attached (then show more info)
    ret += getIdentifier();
    ret += proc.line.code->substr(proc.line.line_pos.first, proc.line.line_pos.second - proc.line.line_pos.first);
    return ret;
}
std::string SyntaxError::getErrorPointerString() const{
    std::string ret;
    ret += std::string((proc.current - proc.line.line_pos.first) + getIdentifier().size(), ' ');
    ret += '^';
    return ret;
}
std::string SyntaxError::getIdentifier() const{
	std::string ret;
	if(!proc.line.file.empty()){
        ret += proc.line.file;
        ret += "(";
        std::ostringstream tmp;
        tmp << proc.line.number;
        ret += tmp.str();
        ret += "): ";
    }
    return ret;
}

/* Left token */
std::string LeftToken::getErrorPointerString() const{
    std::string ret;
    ret += std::string((leftExpression->line.pos.first - leftExpression->line.line_pos.first) + getIdentifier().size(), ' ');
    ret += std::string(leftExpression->line.pos.second - leftExpression->line.pos.first, '^');
    return ret;
}

/* ExecError */
std::string ExecError::getTokenString() const{
    std::string ret;
    //check if a file is attached (then show more info)
    ret += getIdentifier();
    ret += error->line.code->substr(error->line.line_pos.first, error->line.line_pos.second - error->line.line_pos.first);;
    return ret;
}
std::string ExecError::getErrorPointerString() const{
	std::string ret;
    ret += std::string((error->line.pos.first - error->line.line_pos.first) + getIdentifier().size(), ' ');
    ret += std::string(error->line.pos.second - error->line.pos.first, '^');      
    return ret;
}
std::string ExecError::getIdentifier() const{
	std::string ret;
	if(!error->line.file.empty()){
        ret += error->line.file;
        ret += "(";
        std::ostringstream tmp;
        tmp << error->line.number;
        ret += tmp.str();
        ret += "): ";
    }
    return ret;
}

/* UnsupportedOperation */
UnsupportedOperation::UnsupportedOperation(const Token &child): ExecError(child.getParent()->clone()) {
    //saving result of all children (do it now, because after construction, the object should be constant)
    for(std::vector<Expression>::iterator iter = error->children.begin(); iter < error->children.end(); ++iter){
        children.push_back((*iter)->result());
    }
} 
std::string UnsupportedOperation::what() const throw(){
	std::string ret("Cannot do operation ");
    //convert parent to operator
    ret += error->getName();
    ret += " on ";
                
    //loop through all childs
    for(std::vector<Expression>::const_iterator iter = children.begin(); iter < children.end(); ++iter){
    	ret += (*iter)->getName();
        if(iter+2 < children.end()) ret += ", ";
        else if(iter+1 < children.end()) ret += " and ";
	}
    ret += ".";
                
    return ret;
}

/* UndefinedVariable */
std::string UndefinedName::what() const throw(){
    std::string ret("Name ");
    
    ret += "'";
    //cast to name and get value (should always succeed)
    ret += dynamic_cast<const Name&>(*error).name;
    ret += "' is used, before it is defined!";
    
    return ret;
}
