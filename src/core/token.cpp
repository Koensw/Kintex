#include <iostream>

#include "interpreter.h"
#include "token.h"
#include "level.h"

using namespace kintex;

/* get parent */
Token *Token::getParent() const {
	return parent;
}
