#include "name.h"
#include "exception.h"

using namespace kintex;

/* DynamicToken unreachable declarations */
std::ostream &DynamicToken::display(std::ostream&) const { throw UnreachableException(); }
std::string DynamicToken::getName() const { throw UnreachableException(); }
Token &DynamicToken::getContents() { throw UnreachableException(); }

/* Creator unreachable declarations */
Value Creator::result(){
	throw UnreachableException();
}
std::ostream &Creator::display(std::ostream &out) const{
    throw UnreachableException();
}
