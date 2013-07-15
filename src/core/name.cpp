#include <typeinfo>

#include "name.h"
#include "exception.h"
#include "tokens/operands/operands.h"
#include "env.h"

using namespace kintex;

/* Creator unreachable declarations */
Value Creator::result(Environment &env){
	throw UnreachableException();
}
std::ostream &Creator::display(std::ostream &out) const{
    throw UnreachableException();
}
