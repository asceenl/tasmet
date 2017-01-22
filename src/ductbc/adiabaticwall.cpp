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

AdiabaticWall::AdiabaticWall(const TaSystem& sys,
                             const us id,
                             const pb::DuctBc& dbc):
    DuctBc(sys,id,dbc)
    
{
    TRACE(15,"AdiabaticWall(id,sys,dbc)");
    tasmet_assert(dbc.type() == pb::AdiabaticWall,"Wrong type given to constructor");
    
}
AdiabaticWall::AdiabaticWall(const TaSystem& sys,
                             const AdiabaticWall& o):
    DuctBc(sys,o)
{
    TRACE(15,"AdiabaticWall(o)");

}
AdiabaticWall::~AdiabaticWall() {

}
AdiabaticWall* AdiabaticWall::copy(const TaSystem& sys) const {
    return new AdiabaticWall(sys,*this);
}
vd AdiabaticWall::initialSolution() const {
    return vd();
}

void AdiabaticWall::residualJac(arma::subview_col<d>&& residual
                             ) const {

    TRACE(15,"AdiabaticWall::residual()");
    const Duct& duct = getDuct();
    const pb::Duct& dpb = duct.getDuctPb();
    us Ns = sys.Ns();

    if(_side == pb::left) {
        residual.subvec(0,Ns-1) = duct.ut(0);
        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = 
            tasmet_assert(false,"");
        }
    }
    else {
        residual.subvec(0,Ns-1) = duct.ut(-1);
        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = duct.Tt(sys,-1) - _T;
            tasmet_assert(false,"");
        }
    }

}

us AdiabaticWall::getNEqs() const {
    TRACE(15,"AdiabaticWall::getNEqs()");    
    // u = 0
    // dT/dx = 0 --> if htmodel is not Isentropic
    // dTs/dx = 0 => if stempmodel is not Prescribed

    const pb::Duct& dpb = getDuct().getDuctPb();

    bool has_solideq = dpb.stempmodel() != pb::Prescribed;

    us neqs = (has_solideq ? 2: 1);
    if(dpb.htmodel() != pb::Isentropic) neqs+= 1;

    VARTRACE(15,neqs);
    return neqs;
}
void AdiabaticWall::show(us verbosity_level) const {
    TRACE(15,"AdiabaticWall::show()");    
}

//////////////////////////////////////////////////////////////////////
