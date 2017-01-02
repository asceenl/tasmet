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
#include "tasmet_pyeval.h"

Duct::Duct(const us id,const pb::Duct& duct):
    Segment(id,duct.name()),
    Geom(duct)
{
    TRACE(15,"Duct::Duct()");
    const char* invTsfun = "Invalid solid-temperature prescribing function";
    EvaluateFun Tsfun(duct.stempfunc(),invTsfun);
    Tsfun.addGlobalDef("L",duct.length());
    _Tsprescribed = Tsfun(x);
    if(min(_Tsprescribed) < constants::min_T0 ||
       max(_Tsprescribed) > constants::max_T0) {
        throw TaSMETError(invTsfun);
    }


    switch (duct.htmodel()) {
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
    _Tsprescribed(other._Tsprescribed)
{
    // Do something with the equations here
}
Duct* Duct::copy() const {
    return new Duct(*this);
}
Duct::~Duct() {
    // for(Equation* eq: _eqs){
    //     delete eq;
    // }
}
void Duct::residual(const TaSystem& sys,arma::subview_col<d> && residual) const {

    const arma::subview_col<d> sol = sys.getSolution(_id);

    VARTRACE(15,sol(0));
    

}
vd Duct::initialSolution(const TaSystem& sys) const {
    
    vd initsol(getNDofs(sys));

    us vars_per_gp = 5;
    us Ns = sys.Ns();

    const Gas& gas = sys.gas();
    
    for(us i=0;i<ngp();i++){

        // Initial density
        initsol.subvec((i*vars_per_gp+0)*Ns,(i*vars_per_gp+1)*Ns-1) =
            gas.rho0();
        
        // Initial velocity
        initsol.subvec((i*vars_per_gp+1)*Ns,(i*vars_per_gp+2)*Ns-1) =
            0;

        // Initial Temperature
        initsol.subvec((i*vars_per_gp+2)*Ns,(i*vars_per_gp+2)*Ns-1) =
            _Tsprescribed(i);

        // Initial pressure
        initsol.subvec((i*vars_per_gp+3)*Ns,(i*vars_per_gp+4)*Ns-1) =
            gas.p0();
        
        // Initial solid temperature
        initsol.subvec((i*vars_per_gp+4)*Ns,(i*vars_per_gp+5)*Ns-1) =
            _Tsprescribed(i);

    }

    return initsol;

}

us Duct::getNEqs(const TaSystem& sys) const {

    us Ns = sys.Ns();
    us number_eqs = _eqs.size();

    return Ns*number_eqs*ngp();
}
us Duct::getNDofs(const TaSystem& sys) const {
    
    us Ns = sys.Ns();  

    // rho,u,T,p,Ts
    us nvars_per_gp = 5;

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
