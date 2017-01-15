// pressurebc.h
//
// Author: J.A. de Jong 
//
// Description:
// Pressure boundary condition on a side of the duct
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PRESSUREBC_H
#define PRESSUREBC_H
#include "ductbc.h"
#include "ductbc.pb.h"

class TaSystem;
class Variable;

/**
 * A PressureBc is a Duct boundary condition, where the pressure is
 * prescribed on one side of a Duct. Besides the pressure it is
 * necessary to also prescribe the temperature, and if a model is used
 * to thermally interact with the solid, also a solid temperature is
 * prescribed.
 */

class PressureBc: public DuctBc {
    vd _p,_T,_Ts;               /**< Prescribed values for pressure,
                                   temperature and solid temperature */
    pb::DuctSide _side;         /**< Duct side at which this b.c. works */
protected:
    PressureBc(const PressureBc&);
public:
    PressureBc(const us id,
               const TaSystem& sys,
               const pb::DuctBc&);
    ~PressureBc();
    PressureBc* copy() const;

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


#endif // PRESSUREBC_H
//////////////////////////////////////////////////////////////////////
