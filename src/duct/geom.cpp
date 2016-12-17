// geom.cpp
//
// last-edit-by: J.A. de Jong
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "geom.h"
#include "grid.h"
#include "duct.pb.h"
#include "tasmet_pyeval.h"
#include <memory>
#include "tasmet_exception.h"

Geom::Geom(const pb::Duct& duct) {

    // Step one: create the grid
    std::unique_ptr<Grid> grid;
    switch (duct.gridtype()) {
    case pb::Linear:
        grid = std::unique_ptr<Grid>(new LinearGrid(duct.ngp(),
                                                    duct.length()));
        break;
    case pb::BlGrid:
            grid = std::unique_ptr<Grid>(new BlGrid(duct.length(),
                                                    duct.dxb(),
                                                    duct.dxmid()));
        break;
    default:
        return;
        break;
    }

    // Store x
    x = grid->getx();

    EvaluateFun Sfun(duct.area());
    Sfun.addGlobalDef("L",duct.length());
    S = Sfun(x);

    for(auto& Si:S) {
        if(Si <=0){
            throw TaSMETError("Invalid cross-sectional area function");
        }
    }


    EvaluateFun phifun(duct.phi());
    Sfun.addGlobalDef("L",duct.length());
    phi = Sfun(x);

    EvaluateFun rhfun(duct.rh());
    Sfun.addGlobalDef("L",duct.length());
    rh = Sfun(x);

    cshape = duct.cshape();
}
string Geom::shapeString() const {
    return pb::Cshape_Name(cshape);
}

bool Geom::isPrismatic() const {
    bool ok = true;
    d S0 = S(0);
    for(auto& Si : S) {
        ok&= Si==S0;

    }
    return ok;
}
d Geom::getFluidVolume() const {
    d Vf=0;
    us i=0;

    Vf+=0.5*(x(i+1)-x(i))*Sf(i); // First 'cell'

    for(i=1;i<ngp()-1;i++){    // Middle cells
        Vf+=0.5*(x(i)-x(i-1))*Sf(i);
        Vf+=0.5*(x(i+1)-x(i))*Sf(i);
    }

    Vf+=0.5*(x(i)-x(i-1))*Sf(i); // Last 'cell'
    return Vf;
  }
  d Geom::getSolidVolume() const {
    d Vs=0;
    int i=0;

    Vs+=0.5*(x(i+1)-x(i))*Ss(i); // First 'cell'

    for(int i=1;i<ngp()-1;i++){    // Middle cells
        Vs+=0.5*(x(i)-x(i-1))*Ss(i);
        Vs+=0.5*(x(i+1)-x(i))*Ss(i);
    }

    Vs+=0.5*(x(i)-x(i-1))*Ss(i); // Last 'cell'
    return Vs;
  }


//////////////////////////////////////////////////////////////////////
