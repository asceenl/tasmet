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
#include "perfectgas.h"

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

    vd rho,u,T,p,Ts;            // Solution at this gp
    vd rhop,up,Tp,pp,Tsp;       // Solution at next gp

    // Continuity eq residual, momentum, energy, state, solid energy
    vd cont,mom,en,st,sen;
    
    // When we have to solve a solid heat balance
    bool has_solideq = _ductpb.stempmodel() == pb::HeatBalance;
    
    us number_eqs = 4;
    number_eqs += (has_solideq) ? 1 : 0;

    VARTRACE(15,number_eqs);

    us Ns = sys.Ns();
    us eq_offset = 0;           // Equation offset for current gp
    us res_offset = 0;          // Residual offset for current gp
    us res_offsetp = 0;         // Residual offset for next gp

    us gp_jump = number_eqs * Ns; // The jump per gp
    
    rhop = getvart(sys,constants::rho,0);
    up = getvart(sys,constants::u,0);
    Tp = getvart(sys,constants::T,0);
    pp   = getvart(sys,constants::p,0);

    const Gas& gas = sys.gas();

    for(us gp=0;gp<ngp()-1;gp++) {

        eq_offset = gp*Ns*number_eqs;
        res_offset = eq_offset;
        res_offsetp = res_offset + gp_jump;
        
        d dx = x(gp+1)-x(gp);

        // Update the current gp solution
        rho = rhop; u=up; T=Tp; p = pp; Ts=Tsp;

        // Update the next gp solution
        rhop = getvart(sys,constants::rho,gp+1);
        up = getvart(sys,constants::u,gp+1);
        Tp = getvart(sys,constants::T,gp+1);
        pp   = getvart(sys,constants::p,gp+1);

        cont  = ((rhop%up)-(rho%u))/dx;

        mom = (rhop%up%up - rho%u%u + pp - p)/dx;
        
        switch (_ductpb.htmodel()) {
        case pb::Isentropic: {

           d T0 = gas.T0();
           d p0 = gas.p0();
           d rho0 = gas.rho0();
           d gamma0 = gas.gamma(T0,p0);

           en = p/p0 - pow(rho/rho0,gamma0);

        }
            break;
        default:
            tasmet_assert(false,"Not implemented htmodel");
        }

        st = gas.rho(T,p) - rho;

        residual.subvec(eq_offset+0*Ns,eq_offset+1*Ns-1) = cont;
        residual.subvec(eq_offset+1*Ns,eq_offset+2*Ns-1) = mom;
        residual.subvec(eq_offset+2*Ns,eq_offset+3*Ns-1) = en;
        residual.subvec(eq_offset+3*Ns,eq_offset+4*Ns-1) = st;

    }

    eq_offset += number_eqs*Ns;

    // Equation of state for the last node
    st = gas.rho(Tp,pp) - rhop;
    residual.subvec(eq_offset,eq_offset+Ns-1) = st;

    // Two more equations for the last grid point in case
    // the heat transfer model is not a transport equation.
    if(_ductpb.htmodel() == pb::Isentropic) {

        eq_offset += Ns;

        d T0 = gas.T0();
        d p0 = gas.p0();
        d rho0 = gas.rho0();
        d gamma0 = gas.gamma(T0,p0);

        en = p/p0 - pow(rho/rho0,gamma0);

        residual.subvec(eq_offset,eq_offset+Ns-1) = en;
    }

}
vd Duct::getvart(const TaSystem& sys,int varnr,int gp) const {
    TRACE(15,"Duct::getvart()");
    const arma::subview_col<d> sol = sys.getSolution(_id);

    us Ns = sys.Ns();

    // Wraparound
    if(gp<0) gp+=ngp();

    us vars_per_gp = 4;
    vars_per_gp+= (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);
    
    return sol.subvec((gp*vars_per_gp+varnr)*Ns,
                      (gp*vars_per_gp+varnr+1)*Ns-1);
}
vd Duct::getvarx(const TaSystem& sys,int varnr,int t) const {
    vd res(ngp());
    for(us i=0;i<ngp();i++){
        res(i) = getvart(sys,varnr,i)(t);
    }
    return res;
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
            gas.rho0()+.01;
        
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

    // The number of equations per gridpoint. We have: continuity,
    // momentum, energy, and state
    us number_eqs = 4;

    // When we have to solve a solid heat balance
    number_eqs+= (_ductpb.stempmodel() == pb::HeatBalance ?  : 0);

    us neqs = Ns*number_eqs*(ngp()-1);

    // For the last gridpoint, we also have an equation of state
    neqs += Ns;

    // We also have an extra equation for isentropic. For the energy
    // transport equation, this would result in a boundary condition
    if(_ductpb.htmodel() == pb::Isentropic) {
        neqs += Ns;
    }

    VARTRACE(15,neqs);
    return neqs;
}
us Duct::getNDofs(const TaSystem& sys) const {
    TRACE(15,"Duct::getNDofs()");        
    us Ns = sys.Ns();  

    // rho,u,T,p
    us nvars_per_gp = 4;
    // Ts maybe
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
