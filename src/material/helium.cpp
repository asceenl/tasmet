// helium.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "helium.h"

  
d Helium::mu(d T,d p) const {
    return 0.412e-6*pow(T,0.68014);
}
d Helium::kappa(d T,d p) const {
    return 0.0025672*pow(T,0.716);
}
  

//////////////////////////////////////////////////////////////////////
