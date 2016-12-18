// duct.h
//
// Author: J.A. de Jong
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DUCT_H
#define DUCT_H
#include "segment.h"
#include "duct.pb.h"
#include "geom.h"

class Equation;
class Drag;
class Heat;
class TaSystem;

class Duct : public Segment, public Geom {

    Duct(const Duct& other);

    // Drag* _drag = nullptr;
    // Heat* _heat = nullptr;
    std::vector<Equation*> _eqs;

    std::vector<Variable> _rho;
    std::vector<Variable> _u;
    std::vector<Variable> _T;
    std::vector<Variable> _p;
    std::vector<Variable> _Ts;

public:
    Duct(const pb::Duct&);
    virtual Duct* copy() const;
    const Geom& geom() const;
    
    // Solving
    virtual void residual(const TaSystem&,vd&,const us insertion_start) const;

    virtual void updateSolution(const TaSystem&,const vd&);
    virtual getSolution(const TaSystem&,vd& sol,const us insertion_start) const;

    // Return the total number of equations in this segment
    virtual us getNEqs(const TaSystem&) const;
    // Return the total number of DOFS in this segment
    virtual us getNDofs(const TaSystem&) const;

    // Return the current mass in this segment
    virtual d getMass(const TaSystem&) const;
    virtual void dmtotdx(const TaSystem&,vd& dmtotdx,us dof_start) const {}
    
    virtual void show(const TaSystem&,us verbosity_level) const;

    // Reset amplitude data in higher harmonics
    // void resetHarmonics();

    // Fill Jacobian with values from the equations in this
    // segment/connector.
    virtual void jac(const TaSystem&,Jacobian&,us dof_start,us eq_start) const;

};

#endif // DUCT_H
//////////////////////////////////////////////////////////////////////
