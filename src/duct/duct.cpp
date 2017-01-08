// duct.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "duct.h"
#include "tasystem.h"
#include "tasmet_assert.h"
#include "tasmet_evalscript.h"


Duct::Duct(const us id,const pb::Duct& ductpb):
    Segment(id,ductpb.name()),
    Geom(ductpb),
    _ductpb(ductpb)
{
    TRACE(15,"Duct::Duct()");

    const char* invTsfun = "Invalid solid-temperature prescribing function";
    EvaluateFun Tsfun(ductpb.stempfunc(),invTsfun);
    Tsfun.addGlobalDef("L",ductpb.length());
    _Tsprescribed = Tsfun(x);

    if(min(_Tsprescribed) < constants::min_T0 ||
       max(_Tsprescribed) > constants::max_T0) {
        throw TaSMETError(invTsfun);
    }

    switch (ductpb.htmodel()) {
    case pb::Isentropic: {
        break;
    }
    default:
        tasmet_assert(false,"Invalid heat transfer model");
    break;
    }

}
Duct::Duct(const Duct& other):
    Segment(other),
    Geom(other),
    _ductpb(other._ductpb),
    _Tsprescribed(other._Tsprescribed)
{
    // Do something with the equations here
    TRACE(15,"Duct::~Duct");

    

}
Duct* Duct::copy() const {
    return new Duct(*this);
}
Duct::~Duct() {
    TRACE(15,"Duct::~Duct");
    // for(Equation* eq: _eqs){
    //     delete eq;
    // }
}
void Duct::residual(const TaSystem& sys,arma::subview_col<d> && residual) const {

    TRACE(15,"Duct::residual()");
    const arma::subview_col<d> sol = sys.getSolution(_id);

    VARTRACE(15,sol(0));
    

}
vd Duct::initialSolution(const TaSystem& sys) const {
    
    TRACE(15,"Duct::initialSolution()");

    vd initsol(getNDofs(sys));
    VARTRACE(15,initsol.size());

    us vars_per_gp = 4;
    vars_per_gp+= (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);

    us Ns = sys.Ns();

    const Gas& gas = sys.gas();
    
    for(us i=0;i<ngp();i++){
        VARTRACE(15,i);
        // Initial density

        initsol.subvec((i*vars_per_gp+0)*Ns,(i*vars_per_gp+1)*Ns-1) =
            gas.rho0();
        
        // Initial velocity
        initsol.subvec((i*vars_per_gp+1)*Ns,(i*vars_per_gp+2)*Ns-1) =
            0;

        // Initial Temperature
        initsol.subvec((i*vars_per_gp+2)*Ns,(i*vars_per_gp+3)*Ns-1) =
            _Tsprescribed(i);

        // Initial pressure
        initsol.subvec((i*vars_per_gp+3)*Ns,(i*vars_per_gp+4)*Ns-1) =
            gas.p0();
        
        // Initial solid temperature, if not prescribed
        if(_ductpb.stempmodel() != pb::Prescribed) {
            initsol.subvec((i*vars_per_gp+4)*Ns,(i*vars_per_gp+5)*Ns-1) =
                _Tsprescribed(i);
        }
    }

    return initsol;

}

us Duct::getNEqs(const TaSystem& sys) const {
    TRACE(15,"Duct::getNEqs()");    
    us Ns = sys.Ns();
    us number_eqs = 4;

    // When we have to solve a solid heat balance
    number_eqs+= (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);

    return Ns*number_eqs*(ngp()-1);

}
us Duct::getNDofs(const TaSystem& sys) const {
    TRACE(15,"Duct::getNDofs()");        
    us Ns = sys.Ns();  

    // rho,u,T,p,Ts
    us nvars_per_gp = 4;
    nvars_per_gp += (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);

    return Ns*nvars_per_gp*ngp();
}
d Duct::getMass(const TaSystem& sys) const {

    return 0;
}
void Duct::jac(const TaSystem&,Jacobian&,us dof_start,us eq_start) const {

}
void Duct::show(const TaSystem&,us verbosity_level) const {

}


//////////////////////////////////////////////////////////////////////
