// helium.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "helium.h"

  
vd Helium::cp(const vd& T,const vd& p) const {
    return cp(0.0,0.0)*arma::ones(T.size());
}
vd Helium::h(const vd& T,const vd& p) const {
    return cp(T,p)%T;
}
d Helium::mu(d T,d p) const {
    return 0.412e-6*pow(T,0.68014);
}
d Helium::kappa(d T,d p) const {
    return 0.0025672*pow(T,0.716);
}
vd Helium::mu(const vd& T,const vd& p) const {
    return 0.412e-6*pow(T,0.68014);
}
vd Helium::kappa(const vd& T,const vd& p) const {
    return 0.0025672*pow(T,0.716);
}

  

//////////////////////////////////////////////////////////////////////
