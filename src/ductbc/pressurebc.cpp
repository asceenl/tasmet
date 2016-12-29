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

PressureBc::PressureBc(const us id,
                       const TaSystem& sys,
                       const pb::DuctBc& dbc):
    Segment(id,dbc.name())
{
    TRACE(15,"PressureBc(id,sys,dbc)");


    

}
PressureBc::PressureBc(const PressureBc& o):
    Segment(o) {

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

us PressureBc::getNEqs(const TaSystem&) const {
    TRACE(15,"PressureBc::getNEqs()");    


}
void PressureBc::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"PressureBc::show()");    
}
void PressureBc::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"PressureBc::jac()");    

}
//////////////////////////////////////////////////////////////////////
