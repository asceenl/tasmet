// adiabaticwall.h
//
// Author: J.A. de Jong 
//
// Description:
// Adiabatic wall boundary condition on one side of the duct
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ADIABATICWALL_H
#define ADIABATICWALL_H
#include "segment.h"
#include "ductbc.h"

class TaSystem;
class Variable;

/**
 * The adiabaticWall class represents a Duct boundary condition, which
 * blocks the flow and does not allow any axial heat conduction.
 */

class AdiabaticWall: public DuctBc {
    pb::DuctSide _side;         /**< Duct side at which this b.c. works */
    us _duct_id;                /**< ID of Duct for this b.c. */

protected:
    AdiabaticWall(const AdiabaticWall&);
public:
    AdiabaticWall(const us id,
               const TaSystem& sys,
               const pb::DuctBc&);
    ~AdiabaticWall();
    AdiabaticWall* copy() const;

    vd initialSolution(const TaSystem&) const;

    virtual void residual(const TaSystem&,
                          arma::subview_col<d>&& residual
                          ) const;

    // Return the total number of equations in this segment
    virtual us getNEqs(const TaSystem&) const;
    
    // Return the current mass in this segment
    virtual d getMass(const TaSystem&) const { return 0;};
    
    virtual void show(const TaSystem&,us verbosity_level) const;

    // Reset amplitude data in higher harmonics
    // virtual void resetHarmonics() = 0;

    // Fill Jacobian with values from the equations in this
    // segment/connector.
    virtual void jac(const TaSystem&,Jacobian&,us dof_start,us eq_start) const;
                  

};

#endif // ADIABATICWALL_H
//////////////////////////////////////////////////////////////////////
