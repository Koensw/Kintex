#include <iostream>

#include "interpreter.h"
#include "token.h"
#include "table.h"

using namespace kintex;

/* get parent */
Token *Token::getParent() const {
	return parent;
}
