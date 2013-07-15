#include <iostream>

#include "interpreter.h"
#include "token.h"
#include "table.h"
#include "operand.h"

using namespace kintex;

/* get parent */
Token *Token::getParent() const {
	return parent;
}
