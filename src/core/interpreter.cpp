#include <string>
#include <iostream>

#include "operand.h"
#include "interpreter.h"

using namespace kintex;

/* Expression constructors */
Expression::Expression(): Handle<Token>(nullptr) {}
Expression::Expression(Token *tok): Handle<Token>(tok) {}
Expression::Expression(Token &tok): Handle<Token>(tok.clone()) {}
Expression::Expression(const Token &tok): Handle<Token>(tok.clone()) {}

/* Value constructors */
Value::Value(Operand *op): Handle<Operand>(op) {}
Value::Value(Operand &op): Handle<Operand>(op.clone()) {}
Value::Value(const Operand &op): Handle<Operand>(op.clone()) {}

/* Value conversion */
Value::operator Expression() const { return Expression(*ptr); }

/* Statement proxy behaviours */
std::ostream &Statement::display(std::ostream &out) const { return expr->display(out); }
Expression Statement::result() { return expr->result(); }

/* Overloaded token operators */
std::ostream &kintex::operator<<(std::ostream &os, const Expression &expr){
    return expr->display(os);
}
std::ostream &kintex::operator<<(std::ostream &os, const Statement &statement){
    return statement.display(os);
}
