#ifndef KINTEX_OPERAND_H
#define KINTEX_OPERAND_H

#include <iostream>
#include <typeinfo>

#include "token.h"
#include "exception.h"

namespace kintex{
    class Integer;
    class FloatingPoint;
    class Variable;
    class Void;
    
    class Operand: public Token{
    public:
        /** Define operators that should be implemented
         * - throw exception if operator is not implemented;
         */
        /* Add operators */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &operator+=(Operand &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator+=(Integer &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator+=(FloatingPoint &op){throw UnsupportedOperation(*this);}
        /* Subtract operators */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &operator-=(Operand &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator-=(Integer &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator-=(FloatingPoint &op){throw UnsupportedOperation(*this);}
        /* Multiply operators */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &operator*=(Operand &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator*=(Integer &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator*=(FloatingPoint &op){throw UnsupportedOperation(*this);}
        /* Divide operators */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &operator/=(Operand &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator/=(Integer &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator/=(FloatingPoint &op){throw UnsupportedOperation(*this);}
        /* Set operator */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &operator=(Operand &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator=(Integer &op){throw UnsupportedOperation(*this);}
        virtual Operand &operator=(FloatingPoint &op){throw UnsupportedOperation(*this);}
        /* Power operator */
        /* --> DISPATCHING OPERATOR <-- */
        virtual Operand &pow(Operand &op) {throw UnsupportedOperation(*this);}
        virtual Operand &pow(Integer &) {throw UnsupportedOperation(*this);}
        virtual Operand &pow(FloatingPoint &) {throw UnsupportedOperation(*this);}
        
        /* Equality operator */
        /* --> DISPATCHING OPERATOR <-- */
        virtual bool operator==(const Operand &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator==(const Integer &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator==(const FloatingPoint &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator==(const Void &op) const{throw UnsupportedOperation(*this);}
        
        /* Relational operator */
        /* --> DISPATCHING OPERATOR <-- */
        virtual bool operator<(const Operand &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator<(const Integer &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator<(const FloatingPoint &op) const{throw UnsupportedOperation(*this);}
        virtual bool operator<(const Void &op) const{throw UnsupportedOperation(*this);}
        
        /** Result should return copy of itself
         */
        Value result();
        
        /** Redefine pure virtual clone to give an operand back --> else VALUE class is not working */
        virtual Operand *clone() const = 0;
        
    };
    
    class Number: public Operand{
        /* nothing special */
    };
}
#endif
