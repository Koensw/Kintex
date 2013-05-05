#ifndef KINTEX_OPERANDS_H
#define KINTEX_OPERANDS_H

#include <gmpxx.h>
#include <iostream>
#include <sstream>

#include "../../operand.h"
#include "../../interpreter.h"

namespace kintex{
    class Processor;
    class Void;
    
    /* DOUBLE DISPATCH: 
     * Rules:
     * - add *this value to operand
     * - call will be correctly resolved
     * - return operand instead of this in dispatched function
     */
     
     
	//MAX VALUE (always MOD with this number)
    const mpz_class INTEGER_MAX("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
   
    class Integer: public Number{
        friend class FloatingPoint;
        friend class Variable;
    public:
        /* Default constructor */
        Integer(): val(0) {}
        /* FIXME: DEPRECATED create constructor */
        Integer(int value, Token *givenParent = nullptr): val(value) {parent = givenParent;}
        /* Create constructor */
        Integer(mpz_class value, Token *givenParent = nullptr): val(value) {parent = givenParent;}
        /* Display function */
        std::ostream &display(std::ostream &out) const{ 
            out << val;
            return out;
        }
        /* Name function */
        std::string getName() const{ return "integer";}
        /* Clone function */
        Integer *clone() const { return new Integer(*this);}
        /* Create operation --> bind to processor */
        Integer *create(Processor&);
        
        /* Different arithmetic operators */
        Operand &operator+=(Operand &op){ op += *this; return *this;}
        Operand &operator+=(Integer &);
        Operand &operator+=(FloatingPoint &);
        Operand &operator-=(Operand &op){ op -= *this; return *this;}
        Operand &operator-=(Integer &);
        Operand &operator-=(FloatingPoint &);
        Operand &operator*=(Operand &op){ op *= *this; return *this;}
        Operand &operator*=(Integer &);
        Operand &operator*=(FloatingPoint &);
        Operand &operator/=(Operand &op){ op /= *this; return *this;}
        Operand &operator/=(Integer &);
        Operand &operator/=(FloatingPoint &);
        Operand &pow(Operand &op) { op.pow(*this); return *this; }
        Operand &pow(Integer &);
        Operand &pow(FloatingPoint &);
        Operand &mod(Operand &op) { op.mod(*this); return *this; }
        Operand &mod(Integer &);
        
        /* Special set operator */
        Operand &operator=(Operand &op){ op = *this; return *this;}
        Operand &operator=(Integer &);
        Operand &operator=(FloatingPoint &);	
        
        /* Relational operator */
        bool operator==(const Operand &op) const{ return op == *this; }
        bool operator==(const Integer &) const; 
        bool operator==(const FloatingPoint &) const; 
        bool operator==(const Void &) const; 
        bool operator<(const Operand &op) const{ return op < *this; }
        bool operator<(const Integer &) const;
        bool operator<(const FloatingPoint &) const; 
        bool operator<(const Void &) const; 
    private:      
        mpz_class val;
    };
    
    class FloatingPoint: public Number{
        friend class Integer;
        friend class Variable;
    public:
        /* Default constructor */
        FloatingPoint(): val(0) {}
        /* Special conversion constructor */
        FloatingPoint(Integer i): val(i.val.get_d()) {}
        /* Create constructor */
        FloatingPoint(double value): val(value) {}
        /* Display function */
        std::ostream &display(std::ostream &out) const{ 
            out << val;
            return out;
        }
        /* Name function */
        std::string getName() const{ return "floating-point";}
        /* Clone function */
        FloatingPoint *clone() const{ return new FloatingPoint(*this);}
        /* Create operation --> bind to processor */
        FloatingPoint *create(Processor&);
        
        /* Different arithmetic operators */
        Operand &operator+=(Operand &op){ op += *this; return *this;}
        Operand &operator+=(Integer &);
        Operand &operator+=(FloatingPoint &);
        Operand &operator-=(Operand &op){ op -= *this; return *this;}
        Operand &operator-=(Integer &);
        Operand &operator-=(FloatingPoint &);
        Operand &operator*=(Operand &op){ op *= *this; return *this;}
        Operand &operator*=(Integer &);
        Operand &operator*=(FloatingPoint &);
        Operand &operator/=(Operand &op){ op /= *this; return *this;}
        Operand &operator/=(Integer &);
        Operand &operator/=(FloatingPoint &);
        Operand &pow(Operand &op) { op.pow(*this); return *this; }
        Operand &pow(Integer &);
        Operand &pow(FloatingPoint &);
        Operand &mod(Operand &op) { op.mod(*this); return *this; }
        
        /* Special set operator */
        Operand &operator=(Operand &op){ op = *this; return *this;}
        Operand &operator=(Integer &);
        Operand &operator=(FloatingPoint &);
        
        /* Relational operator */
        bool operator==(const Operand &op) const{ return op == *this; }
        bool operator==(const Integer &) const; 
        bool operator==(const FloatingPoint &) const; 
        bool operator==(const Void &) const; 
        bool operator<(const Operand &op) const{ return op < *this; }
        bool operator<(const Integer &) const;
        bool operator<(const FloatingPoint &) const; 
        bool operator<(const Void &) const; 
    private:      
        double val;
    };
    
    class Void: public Operand{
    public:
        /* Default constructor */
        Void() {}
        /* Use constructor */
        Void(Token *givenParent) {parent = givenParent;}
        /* Display function */
        std::ostream &display(std::ostream &out) const{ 
            out << "(void)";
            return out;
        }
        /* Name function */
        std::string getName() const{ return "void";}
        /* Clone function */
        Void *clone() const { return new Void(*this);}
        /* Create operation --> bind to processor */
        Void *create(Processor&);
        
        /* Relational operator */
        bool operator==(const Operand &op) const{ return op == *this; }
        bool operator==(const Integer &) const; 
        bool operator==(const FloatingPoint &) const; 
        bool operator==(const Void &) const; 
        bool operator<(const Operand &op) const{ return op < *this; }
        bool operator<(const Integer &) const;
        bool operator<(const FloatingPoint &) const; 
        bool operator<(const Void &) const; 
    };
}

#endif
