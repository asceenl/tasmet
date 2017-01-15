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

PressureBc::PressureBc(const us id,
                       const TaSystem& sys,
                       const pb::DuctBc& dbc):
    DuctBc(id,dbc)
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
PressureBc::PressureBc(const PressureBc& o):
    DuctBc(o),_p(o._p),_T(o._T) {

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

void PressureBc::residual(const TaSystem& sys,
                          arma::subview_col<d>&& residual
                          ) const {

    TRACE(15,"PressureBc::residual()");

    const pb::Duct& dpb = getDuct(sys).getDuctPb();
    us Ns = sys.Ns();

    const Duct& duct = getDuct(sys);
    if(_side == pb::left) {
        residual.subvec(0,Ns-1) = duct.pt(sys,0) - _p;

        if(dpb.htmodel() != pb::Isentropic) {
            residual.subvec(Ns,2*Ns-1) = duct.Tt(sys,0) - _T;
        }
    }
    else {
        residual.subvec(0,Ns-1) = duct.pt(sys,-1) - _p;

        if(dpb.htmodel() != pb::Isentropic) {
            residual.subvec(Ns,2*Ns-1) = duct.Tt(sys,-1) - _T;
        }
    }
}

us PressureBc::getNEqs(const TaSystem& sys) const {
    TRACE(15,"PressureBc::getNEqs()");    

    // We provide equations for
    // p = prescribed
    // T = prescribed, if htmodel of duct is not Isentropic
    // Ts = prescribed, if stempmodel of duct is not Prescribed

    const pb::Duct& dpb = getDuct(sys).getDuctPb();

    bool has_solideq = dpb.stempmodel() != pb::Prescribed;

    us neqs = sys.Ns()*(has_solideq ? 2: 1);
    if(dpb.htmodel() != pb::Isentropic) neqs+= sys.Ns();

    VARTRACE(15,neqs);
    return neqs;
}
void PressureBc::show(const TaSystem&,us verbosity_level) const {
    TRACE(15,"PressureBc::show()");    
}
void PressureBc::jac(const TaSystem&,Jacobian&,
                     us dof_start,us eq_start) const {

    TRACE(15,"PressureBc::jac()");    

}
//////////////////////////////////////////////////////////////////////
