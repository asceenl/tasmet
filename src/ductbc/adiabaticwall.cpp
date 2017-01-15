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
    const pb::Duct& dpb = getDuct(sys).getDuctPb();
    us Ns = sys.Ns();

    const Duct& duct = getDuct(sys);
    if(_side == pb::left) {
        residual.subvec(0,Ns-1) = duct.ut(sys,0);
        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = 
            tasmet_assert(false,"");
        }
    }
    else {
        residual.subvec(0,Ns-1) = duct.ut(sys,-1);
        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = duct.Tt(sys,-1) - _T;
            tasmet_assert(false,"");
        }
    }

}

us AdiabaticWall::getNEqs(const TaSystem& sys) const {
    TRACE(15,"AdiabaticWall::getNEqs()");    
    // u = 0
    // dT/dx = 0 --> if htmodel is not Isentropic
    // dTs/dx = 0 => if stempmodel is not Prescribed

    const pb::Duct& dpb = getDuct(sys).getDuctPb();

    bool has_solideq = dpb.stempmodel() != pb::Prescribed;

    us neqs = sys.Ns()*(has_solideq ? 2: 1);
    if(dpb.htmodel() != pb::Isentropic) neqs+= sys.Ns();

    VARTRACE(15,neqs);
    return neqs;
}
void AdiabaticWall::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"AdiabaticWall::show()");    
}
void AdiabaticWall::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"AdiabaticWall::jac()");    

}
//////////////////////////////////////////////////////////////////////
