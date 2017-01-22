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
#include "tasmet_constants.h" // For the variable nrs

class Equation;
class Drag;
class Heat;
class TaSystem;

class Duct : public Segment, public Geom {

    // Drag* _drag = nullptr;
    // Heat* _heat = nullptr;

    pb::Duct _ductpb;
    vd _Tsprescribed;

protected:
    Duct(const TaSystem&,const Duct&);
public:
    Duct(const TaSystem&,const us id,const pb::Duct&);
    ~Duct();

    vd Tsprescribed() const { return _Tsprescribed;}

    // Initialize the solution to something sensible
    vd initializeSolution(const TaSystem& sys);

    virtual Duct* copy(const TaSystem&) const;
    const Geom& geom() const;
    
    const pb::Duct& getDuctPb() const { return _ductpb;}

    /** 
     * Returns the dof nr for the given variable at given gridpoint.
     *
     * @return The index of the degree of freedom
     */
    PosId getDof(int varnr,int gp) const;

    /// Postprocessing functions
    vd rhox(int t) const { return getvarx(constants::rho,t);}
    vd ux(int t) const { return getvarx(constants::u,t); }
    vd Tx(int t) const { return getvarx(constants::T,t); }
    vd px(int t) const { return getvarx(constants::p,t); }
    vd Tsx(int t) const { return getvarx(constants::Ts,t); }

    vd rhot(int gp) const { return getvart(constants::rho,gp); }
    vd ut(int gp) const { return getvart(constants::u,gp); }
    vd Tt(int gp) const { return getvart(constants::T,gp); }
    vd pt(int gp) const { return getvart(constants::p,gp); }
    vd Tst(int gp) const { return getvart(constants::Ts,gp); }

    /// Obtain variable as a function of time for a given grid point
    vd getvart(int varnr,int gp) const;

    /// Obtain variable as a function of position, for a given time
    /// instance
    vd getvarx(int varnr,int t) const;

    d getvartx(int t,int gp) const;
    // Solving
    virtual void residualJac(arma::subview_col<d>&& residual) const;

    vd initialSolution() const;

    // virtual void getSolution(const TaSystem&,const us insertion_start) const;

    // Return the total number of equations in this segment
    virtual us getNEqs() const;
    // Return the total number of DOFS in this segment
    virtual us getNDofs() const;

    // Return the current mass in this segment
    virtual d getMass() const;
    virtual void dmtotdx(vd& dmtotdx,us dof_start) const {}
    
    virtual void show(us verbosity_level) const;

    // Reset amplitude data in higher harmonics
    // void resetHarmonics();

};

#endif // DUCT_H
//////////////////////////////////////////////////////////////////////
