#pragma once
#ifndef _DRAG_H_
#define _DRAG_H_
#include "tasmet_types.h"

class DragResistance{
public:
    virtual ~DragResistance(){}
    virtual vd drag(us gp) const;
    virtual tasystem::JacRow dDrag(const Cell& v) const;
    // virtual dmat drhoi(const Cell& v) const;
    // virtual dmat dpi(const Cell& v) const;

    // Provide a vector of shear wave numbers for each frequency. The
    // length of this vector is Nf+1, and obviously, the first     // of this vector is always zero.
    vd shearWaveNumber(const Cell& v) const;
      
    static d rho0(const Cell& v);
    static d mu0(const Cell& v);
};
    
#endif /* _DRAG_H_ */





















