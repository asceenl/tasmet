#pragma once
#ifndef _GRID_H_
#define _GRID_H_
#include "tasmet_enum.h"
#include "tasmet_types.h"


class Grid{
public:
    Grid& operator=(const Grid& g)=delete;
    virtual vd getx() const=0;
    virtual ~Grid(){}
};

class LinearGrid: public Grid{
    us ngp;
    d L;			// Length of the Duct
public:
    LinearGrid(us ngp,d L);
    LinearGrid(const LinearGrid& g):LinearGrid(g.ngp,g.L){}
    vd getx() const {return linspace(0,L,ngp);}
};

// Boundary layer grid.
// L: length of grid
// dxb: boundary layer grid spacing (minimal grid spacing)
// xb: boundary layer tickness
// dxmid: spacing in the middle part
  

class BlGrid:public Grid{
    d L,dxb,dxmid;
public:
    BlGrid(d L,d dxb,d dxmid);
    vd getx() const;
};


#endif /* _GRID_H_ */

