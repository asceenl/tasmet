// adiabaticwall.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "adiabaticwall.h"
#include "ductbc.pb.h"
#include "tasmet_tracer.h"
#include "tasmet_assert.h"


AdiabaticWall::AdiabaticWall(const us id,
                       const TaSystem& sys,
                       const pb::DuctBc& dbc):
    Segment(id,dbc.name()),
    _side(dbc.side()),
    _duct_id(dbc.duct_id())
{
    TRACE(15,"AdiabaticWall(id,sys,dbc)");
    tasmet_assert(dbc.type() == pb::AdiabaticWall,"Wrong type given to constructor");

    
}
AdiabaticWall::AdiabaticWall(const AdiabaticWall& o):
    Segment(o) {

    TRACE(15,"AdiabaticWall(o)");

}
AdiabaticWall::~AdiabaticWall() {

}
AdiabaticWall* AdiabaticWall::copy() const {
    return new AdiabaticWall(*this);
}
vd AdiabaticWall::initialSolution(const TaSystem& sys) const {
    return vd();
}

void AdiabaticWall::residual(const TaSystem&,
                          arma::subview_col<d>&& residual
                          ) const {

    TRACE(15,"AdiabaticWall::residual()");


}

us AdiabaticWall::getNEqs(const TaSystem&) const {
    TRACE(15,"AdiabaticWall::getNEqs()");    


}
void AdiabaticWall::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"AdiabaticWall::show()");    
}
void AdiabaticWall::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"AdiabaticWall::jac()");    

}
//////////////////////////////////////////////////////////////////////
