// pressurebc.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "pressurebc.h"
#include "ductbc.pb.h"
#include "tasmet_tracer.h"
#include "tasystem.h"
#include "duct.h"

PressureBc::PressureBc(const us id,
                       const TaSystem& sys,
                       const pb::DuctBc& dbc):
    DuctBc(id,dbc)
{
    TRACE(15,"PressureBc(id,sys,dbc)");

}
PressureBc::PressureBc(const PressureBc& o):
    DuctBc(o) {

    TRACE(15,"PressureBc(o)");

}
PressureBc::~PressureBc() {

}
PressureBc* PressureBc::copy() const {
    return new PressureBc(*this);
}
vd PressureBc::initialSolution(const TaSystem& sys) const {
    return vd();
}

void PressureBc::residual(const TaSystem&,
                          arma::subview_col<d>&& residual
                          ) const {

    TRACE(15,"PressureBc::residual()");


}

us PressureBc::getNEqs(const TaSystem& sys) const {
    TRACE(15,"PressureBc::getNEqs()");    
    // p = x
    // T = x
    // This one only if the duct solves for solid
    // Ts = x => 3 equations
    bool has_solideq = getDuct(sys).getDuctPb().stempmodel() != pb::Prescribed;
    return sys.Ns()*(has_solideq ? 3: 2);
}
void PressureBc::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"PressureBc::show()");    
}
void PressureBc::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"PressureBc::jac()");    

}
//////////////////////////////////////////////////////////////////////
