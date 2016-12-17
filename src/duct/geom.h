// geom.h
//
// Author: J.A. de Jong
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GEOM_H
#define GEOM_H

#include "tasmet_constants.h"
#include "tasmet_types.h"
#include "duct.pb.h"

class Geom{

public:
    // Parse geometry from duct
    Geom(const pb::Duct& duct);

    // Public members
    vd x,S,phi,rh;
    pb::Cshape cshape;

    // Shape keyword
    string shapeString() const;

    bool isPrismatic() const;

    us ngp() const {return x.size();}
    d L() const {return x(x.size()-1);}
    d Sleft() const {return S(0);}
    d Sright() const {return S(ngp()-1);}

    d Sf(us i) const {return S(i)*phi(i);}       // Fluid-occupied cross-sectional area of cell-wall
    d Ss(us i) const {return (1-phi(i))*S(i);}       // Solid-occupied cross-sectional area

    d getFluidVolume() const;
    d getSolidVolume() const;

};                            /* class Geom */

#endif /* _GEOM_H_ */
