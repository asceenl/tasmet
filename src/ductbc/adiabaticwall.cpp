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


void AdiabaticWall::residualJac(SegPositionMapper& residual,
                                SegJacRows& jac) const {

    TRACE(15,"AdiabaticWall::residualJac()");
    const Duct& duct = getDuct();
    const pb::Duct& dpb = duct.getDuctPb();
    us Ns = sys.Ns();

    us i=0;

    if(_dbc.side() == pb::left) {
        *residual.at(i) = duct.ut(0);
        jac.at(i)[duct.getDof(constants::u,0)].eye(Ns,Ns);

        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = 
            tasmet_assert(false,"");
        }
    }
    else if(_dbc.side() == pb::right) {
        TRACE(25,"AdWall RIGHT");
        *residual.at(i) = duct.ut(-1);
        jac.at(i)[duct.getDof(constants::u,-1)].eye(Ns,Ns);
        if(dpb.htmodel() != pb::Isentropic) {
            // TODO: Put the boundary condition of zero heat flux here
            // residual.subvec(Ns,2*Ns-1) = duct.Tt(sys,-1) - _T;
            tasmet_assert(false,"");
        }
    }
    else {
        throw TaSMETError("Illegal side parameter given in AdiabaticWall.");
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
