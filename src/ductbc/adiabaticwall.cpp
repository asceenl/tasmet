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
#include "tasystem.h"
#include "duct.h"

AdiabaticWall::AdiabaticWall(const us id,
                       const TaSystem& sys,
                       const pb::DuctBc& dbc):
    DuctBc(id,dbc)
    
{
    TRACE(15,"AdiabaticWall(id,sys,dbc)");
    tasmet_assert(dbc.type() == pb::AdiabaticWall,"Wrong type given to constructor");

    
}
AdiabaticWall::AdiabaticWall(const AdiabaticWall& o):
    DuctBc(o)
{
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

void AdiabaticWall::residual(const TaSystem& sys,
                             arma::subview_col<d>&& residual
                             ) const {

    TRACE(15,"AdiabaticWall::residual()");

}

us AdiabaticWall::getNEqs(const TaSystem& sys) const {
    TRACE(15,"AdiabaticWall::getNEqs()");    
    // u = 0
    // dT/dx = 0
    // dTs/dx = 0 => 3 eqs
    bool has_solideq = getDuct(sys).getDuctPb().stempmodel() != pb::Prescribed;
    return sys.Ns()*(has_solideq ? 3: 2);
}
void AdiabaticWall::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"AdiabaticWall::show()");    
}
void AdiabaticWall::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"AdiabaticWall::jac()");    

}
//////////////////////////////////////////////////////////////////////
