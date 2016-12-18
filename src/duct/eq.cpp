#include "ductequation.h"
#include "cell.h"


namespace duct{
  vd Equation::getp0t() const {
    TRACE(0,"Equation::getp0t()");
    return v.gc->p0()*vd(v.gc->Ns(),fillwith::ones);
  }    
  dmat Equation::eye() const {
    TRACE(15,"Equation::eye()");
    return arma::eye(v.gc->Ns(),v.gc->Ns());}
  dmat Equation::eye(const Cell& v) {
    TRACE(15,"Equation::eye()");
    return arma::eye(v.gc->Ns(),v.gc->Ns());}
  vd Equation::zeros() const {return vd(v.gc->Ns(),fillwith::zeros);}

} // namespace duct
