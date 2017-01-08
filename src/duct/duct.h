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

    // Drag* _drag = nullptr;
    // Heat* _heat = nullptr;
    std::vector<Equation*> _eqs;
    pb::Duct _ductpb;
    vd _Tsprescribed;



protected:
    Duct(const Duct&);
public:
    Duct(const us id,const pb::Duct&);
    ~Duct();

    vd Tsprescribed() const { return _Tsprescribed;}

    // Initialize the solution to something sensible
    vd initializeSolution(const TaSystem& sys);

    virtual Duct* copy() const;
    const Geom& geom() const;
    
    const pb::Duct& getDuctPb() const { return _ductpb;}

    // Solving
    virtual void residual(const TaSystem&,arma::subview_col<d>&& residual) const;

    vd initialSolution(const TaSystem&) const;

    // virtual void getSolution(const TaSystem&,const us insertion_start) const;

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
