// air.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "air.h"

d Air::kappa(d T,d p) const {
    us nfac = _kappac.size();
    vd powfac(nfac);
        
    for(us i=0;i<nfac;i++)
        powfac(i) = pow(T,i);
        
    return dot(_kappac,powfac);
}
d Air::mu(d T,d p) const {
    us nfac = _muc.size();
    vd powfac(nfac);
        
    for(us i=0;i<nfac;i++)
        powfac(i) = pow(T,i);
        
    return dot(_muc,powfac);
}


//////////////////////////////////////////////////////////////////////
