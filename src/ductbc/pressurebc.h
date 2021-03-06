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
protected:
    PressureBc(const TaSystem&,const PressureBc&);
public:

    PressureBc(const TaSystem& sys,
               const us id,
               const pb::DuctBc&);
    
    ~PressureBc();
    
    PressureBc* copy(const TaSystem&) const;

    virtual void residualJac(SegPositionMapper& residual,
                             SegJacRows& jac) const;

    // Return the total number of equations in this segment
    virtual us getNEqs() const;
    
    // Return the current mass in this segment
    virtual d getMass() const { return 0;};
    
    virtual void show(us verbosity_level) const;

    // Reset amplitude data in higher harmonics
    // virtual void resetHarmonics() = 0;


};


#endif // PRESSUREBC_H
//////////////////////////////////////////////////////////////////////
