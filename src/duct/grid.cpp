#include "grid.h"
#include "tasmet_tracer.h"
#include "tasmet_exception.h"
#include "tasmet_constants.h"
#include <sstream>

LinearGrid::LinearGrid(us ngp,d L):
    ngp(ngp),
    L(L)
{
    std::stringstream error;
    if(ngp<constants::min_ngp) {
        error << "Number of gridpoints is lower than minimum. Minimum is: ";
        error << constants::min_ngp;
        throw TaSMETError(error);
    }
    else if(ngp>constants::max_ngp) {
        error << "Maximum number of gridpoints exceeded. Maximum is: ";
        error << constants::max_ngp;
        throw TaSMETError(error);
    }
    if(L<=0){
        throw TaSMETError("Illegal length chosen.");
    }
}
BlGrid::BlGrid(d L,d dxb,d dxmid):
    L(L),dxb(dxb),dxmid(dxmid)
{
    TRACE(15,"BlGrid::BlGrid()");
    if(L<=0 || dxb<=0 || dxmid < dxb){
        throw TaSMETError("Illegal length chosen.");
    }
    
}

vd BlGrid::getx() const {
    TRACE(15,"BlGrid::getx()");

    d delta=2*acosh(sqrt(dxmid/dxb));

    d dxdxi0=0.5*L*(delta/tanh(delta/2))*(1-tanh(delta/2));
    d dxi=dxb/dxdxi0;
    us N=ceil(1/dxi);

    vd xi=linspace(0,1,N);
    
    return L*0.5*(1+tanh(delta*(xi-0.5))/tanh(delta/2));

    
}
