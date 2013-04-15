#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <utility>
#include <memory>
#include <cctype>

#include "kintex.h"
#include "core/exception.h"
#include "core/interpreter.h"
#include "core/token.h"
#include "core/processor.h"

using namespace kintex;

/* 
 * Implementation of the interpreter: the communication class with the programmer-user
 */

/* Parse a string */
Interpreter::StatementList Interpreter::parse(std::string str, std::string name){
    //convert to iterators and parse
    return parse(str.begin(), str.end(), name);
}

/* Parse a input stream (like standard input or a file) */
Interpreter::StatementList Interpreter::parse(std::istream &in, std::string name){
    //build iterators
    std::istream_iterator<char> begin(in);
    std::istream_iterator<char> end;
    //convert to string (because an random-access iterator is needed)
    std::string parseString(begin, end);
    //parse string
    return parse(parseString, name);
}

/* Parse one line from an input stream */
Interpreter::StatementList Interpreter::parseLine(std::istream &in, std::string name){
    std::string parseString;
    //use getline to get the string
    std::getline(in, parseString);
    //parse string
    return parse(parseString, name);
}

/* Private parser used by other kintex parsers */
template<typename Iter> Interpreter::StatementList Interpreter::parse(Iter begin, Iter end, std::string file_name){
    StatementList ret;
	
	std::string code(begin, end);	
	Processor proc(tokenList, sgroup, code, file_name);
	while(proc.isStatementLeft()){
		Expression e = proc.getNextStatement();
		ret.push_back(e);
	}
    return ret;
}