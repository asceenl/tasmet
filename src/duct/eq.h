// eq.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef EQ_H
#define EQ_H

#include "constants.h"


class JacRow;
class JacCol;

class Duct;
class WeightFactors;
  
class Equation{
protected:
    us dofnr;
public:
    Equation(const Cell& v):v(v){TRACE(15,"Equation(v)");}
    virtual enum EqType getType() const=0;

    // Return an eye of the right size:
    dmat eye() const;
    static dmat eye(const Cell&);
    vd zeros() const;
    virtual tasystem::JacRow jac() const=0;		// Returns the local Jacobian of this equation
    virtual vd error() const=0;
    virtual void show() const=0;
    virtual void domg(vd&) const {/* Placeholder */}

    vd getp0t() const;
    vd getT0t() const;
    virtual ~Equation(){}
private:
    vd nu(const Cell&) const;			// Function of d^2p/dx^2
};				// class Equation


#endif // EQ_H
//////////////////////////////////////////////////////////////////////
