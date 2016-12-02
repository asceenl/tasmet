// nitrogen.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "nitrogen.h"
  
d Nitrogen::kappa(d T,d p) const {
    return _kappac(1)*T+_kappac(0);
}

d Nitrogen::mu(d T,d p) const {
    return (0.01781/1000)*(411.55/(T+111))*pow(T/300.55,1.5);
}
  

//////////////////////////////////////////////////////////////////////
