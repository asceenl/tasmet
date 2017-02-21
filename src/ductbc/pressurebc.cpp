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
#include "tasmet_evalscript.h"
#include "perfectgas.h"
#include "adiabatictemp.h"

PressureBc::PressureBc(const TaSystem& sys,
                       const us id,
                       const pb::DuctBc& dbc):
    DuctBc(sys,id,dbc)
{
    TRACE(15,"PressureBc(id,sys,dbc)");
    vd time = sys.timeInstances();
    VARTRACE(15,time);
    
    EvaluateFun pfun(dbc.pressure(),
                     "Error in evaluating prescribed pressure",
                     "t");
                     
    pfun.addGlobalDef("f",sys.getfreq());
    pfun.addGlobalDef("omg",sys.getomg());
    pfun.addGlobalDef("p0",sys.gas().p0());
    pfun.addGlobalDef("T0",sys.gas().T0());

    _p = pfun(time);

    if(dbc.isentropic()) {
        auto& gas = dynamic_cast<const PerfectGas&>(sys.gas());
        _T = adiabaticTemp(gas,_p);
    }

    else {
        TRACE(25,"PressureBc not Isentropic");
        EvaluateFun Tfun(dbc.temperature(),
                         "Error in evaluating prescribed temperature",
                         "t");
                     
        Tfun.addGlobalDef("f",sys.getfreq());
        Tfun.addGlobalDef("omg",sys.getomg());
        Tfun.addGlobalDef("p0",sys.gas().p0());
        Tfun.addGlobalDef("T0",sys.gas().T0());
        _T = Tfun(time);

    }

}
PressureBc::PressureBc(const TaSystem& sys,
                       const PressureBc& o):
    DuctBc(sys,o),
    _p(o._p),
    _T(o._T),
    _Ts(o._Ts)
{
    TRACE(15,"PressureBc(o)");
}
PressureBc::~PressureBc() {
    
}
PressureBc* PressureBc::copy(const TaSystem& sys) const {
    return new PressureBc(sys,*this);
}

void PressureBc::residualJac(SegPositionMapper& residual,
                             SegJacRows& jacrows) const {

    TRACE(15,"PressureBc::residualJac()");

    const pb::Duct& dpb = getDuct().getDuctPb();
    us Ns = sys.Ns();

    // VARTRACE(25,_T-sys.gas().T0);
    
    const Duct& duct = getDuct();
    if(_dbc.side() == pb::left) {
        TRACE(25,"PressureBc LEFT");
        *residual.at(0) = duct.pt(0) - _p;
        jacrows.at(0)[duct.getDof(constants::p,0)].eye(Ns,Ns);
        if(dpb.htmodel() != pb::Isentropic) {
            tasmet_assert(false,"");
            // residual.subvec(Ns,2*Ns-1) = duct.Tt(0) - _T;
        }
    }
    else if(_dbc.side() == pb::right) {
        TRACE(25,"PB RIGHT");
        *residual.at(0) = duct.pt(-1) - _p;
        jacrows.at(0)[duct.getDof(constants::p,-1)].eye(Ns,Ns);
        if(dpb.htmodel() != pb::Isentropic) {
            tasmet_assert(false,"");
            // residual.subvec(Ns,2*Ns-1) = duct.Tt(-1) - _T;
        }
    }
    else {
        throw TaSMETError("Illegal side parameter given in PressureBc.");
    }
}

us PressureBc::getNEqs() const {
    TRACE(15,"PressureBc::getNEqs()");    

    // We provide equations for
    // p = prescribed
    // T = prescribed, if htmodel of duct is not Isentropic
    // Ts = prescribed, if stempmodel of duct is not Prescribed

    const pb::Duct& dpb = getDuct().getDuctPb();

    bool has_solideq = dpb.stempmodel() != pb::Prescribed;

    us neqs = (has_solideq ? 2: 1);
    if(dpb.htmodel() != pb::Isentropic) neqs+= 1;

    VARTRACE(15,neqs);
    return neqs;
}
void PressureBc::show(us verbosity_level) const {
    TRACE(15,"PressureBc::show()");    
}

//////////////////////////////////////////////////////////////////////
