// segment.h
//
// Author: J.A. de Jong 
//
// Description: The Segments class is the base class for
// both the normal segments and the connectors. The main difference
// between a segment and a connector is, that a connector only provide
// equations. A normal segment contains both. All
// common stuff, such as a name, initialization
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H

#include "tasmet_types.h"
#include "tasmet_exception.h"
// #include "phaseconstraint.h"

class Jacobian;
class GlobalConf;
class TaSystem;

class Segment{

    // User identifier
    std::string _name;

protected:
    Segment(const std::string& name): _name(name) {}
    Segment(const Segment& o): Segment(o._name){}
    Segment& operator=(const Segment&)=delete;
public:
    virtual ~Segment(){}

    virtual Segment* copy() const = 0;
    
    // Get and set name
    const std::string& getName() const{return _name;} // This one is just the name
    void setName(const std::string& name){ _name = name; } // This one is just the name
    
    // Tell a TaSystem whether this Segment arbitrates Mass or
    // not. The special return value of -1 tells it does not. If it
    // does, the derived class should return which equation should be
    // overwritten with the mass arbitration equation.
    virtual int arbitrateMassEq() const {return -1;}
    virtual void residual(const TaSystem&,vd&,const us insertion_start) const=0;

    virtual void updateSolution(const TaSystem&,const vd&) = 0;
    virtual getSolution(const TaSystem&,vd& sol,const us insertion_start) const = 0;

    // Return the total number of equations in this segment
    virtual us getNEqs(const TaSystem&) const { return 0;}
    // Return the total number of DOFS in this segment
    virtual us getNDofs(const TaSystem&) const { return 0;}

    // Return the current mass in this segment
    virtual d getMass(const TaSystem&) const = 0;
    virtual void dmtotdx(const TaSystem&,vd& dmtotdx,us dof_start) const {}
    
    virtual void show(const TaSystem&,us verbosity_level) const=0;

    // Reset amplitude data in higher harmonics
    // virtual void resetHarmonics() = 0;

    // Fill Jacobian with values from the equations in this
    // segment/connector.
    virtual void jac(const TaSystem&,Jacobian&,us dof_start,us eq_start) const=0;

};


#endif // SEGMENT_H
//////////////////////////////////////////////////////////////////////
