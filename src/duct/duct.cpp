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
#include "../export/export.h"

Duct::Duct(const TaSystem& sys,
           const us id,
           const pb::Duct& ductpb):
    Segment(sys,id,ductpb.name()),
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
Duct::Duct(const TaSystem& sys,const Duct& other):
    Segment(sys,other),
    Geom(other),
    _ductpb(other._ductpb),
    _Tsprescribed(other._Tsprescribed)
{
    // Do something with the equations here
    TRACE(15,"Duct::Duct");


}
Duct* Duct::copy(const TaSystem& sys) const {
    return new Duct(sys,*this);
}
Duct::~Duct() {
    TRACE(15,"Duct::~Duct");
    // for(Equation* eq: _eqs){
    //     delete eq;
    // }
}
void Duct::residualJac(SegPositionMapper& residual,
                       SegJacRows& jacrows) const {

    TRACE(15,"Duct::residualJac()");
    
    vd rho,u,T,p,Ts;            // Solution at this gp
    vd rhop,up,Tp,pp,Tsp;       // Solution at next gp

    // Continuity eq residual, momentum, energy, state, solid energy
    vd cont,mom,en,st,sen;
    
    // When we have to solve a solid heat balance
    bool has_solideq = _ductpb.stempmodel() == pb::HeatBalance;
    
    us number_eqs = 4;
    number_eqs += (has_solideq) ? 1 : 0;

    // rho,u,T,p
    // Ts maybe
    us nvars_per_gp = (_ductpb.stempmodel()
                       == pb::HeatBalance ? 5 : 4);

    VARTRACE(15,number_eqs);

    us Ns = sys.Ns();

    rhop = getvart(constants::rho,0);
    up = getvart(constants::u,0);
    Tp = getvart(constants::T,0);
    pp = getvart(constants::p,0);

    const Gas& gas = sys.gas();

    us i=0;

    for(us gp=0;gp<ngp()-1;gp++) {

        d dx = x(gp+1)-x(gp);

        // Update the current gp solution
        rho = rhop; u=up; T=Tp; p = pp; Ts=Tsp;

        // Update the next gp solution
        rhop = getvart(constants::rho,gp+1);
        up = getvart(constants::u,gp+1);
        Tp = getvart(constants::T,gp+1);
        pp   = getvart(constants::p,gp+1);

        cont  = ((rhop%up)-(rho%u))/dx;
        // cont += sys.DDTtd()*rho;
        
        *residual.at(i) = cont;

        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::u}] =
            -diagmat(rho)/dx;
        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::rho}] =
            -diagmat(u)/dx;
        jacrows.at(i)[{_id,(gp+1)*nvars_per_gp+constants::rho}] =
            diagmat(up)/dx;
        jacrows.at(i)[{_id,(gp+1)*nvars_per_gp+constants::u}] =
            diagmat(rhop)/dx;

        i++;

        mom = (rhop%up%up - rho%u%u + pp - p)/dx;
        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::u}] =
            -diagmat(rho%u)/dx;
        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::rho}] =
            -diagmat(u%u)/dx;
        jacrows.at(i)[{_id,(gp+1)*nvars_per_gp+constants::rho}] =
            diagmat(u%u)/dx;
        jacrows.at(i)[{_id,(gp+1)*nvars_per_gp+constants::u}] =
            diagmat(up%rhop)/dx;
        jacrows.at(i)[{_id,(gp)*nvars_per_gp+constants::p}] =
            -eye(Ns,Ns)/dx;
        jacrows.at(i)[{_id,(gp+1)*nvars_per_gp+constants::p}] =
            eye(Ns,Ns)/dx;
        *residual.at(i) = mom;

        i++;
        
        switch (_ductpb.htmodel()) {
        case pb::Isentropic: {

           d T0 = gas.T0();
           d p0 = gas.p0();
           d rho0 = gas.rho0();
           d gamma0 = gas.gamma(T0,p0);

           en = p/p0 - pow(rho/rho0,gamma0);

           *residual.at(i) = en;

           jacrows.at(i)[{_id,gp*nvars_per_gp+constants::p}] =
               eye(Ns,Ns)/p0;
           jacrows.at(i)[{_id,gp*nvars_per_gp+constants::rho}] =
               -(gamma0/rho0)*diagmat(pow(rho/rho0,gamma0-1));

        }
            break;
        default:
            tasmet_assert(false,"Not implemented htmodel");
        }

        i++;

        // Equation of state
        st = gas.rho(T,p) - rho;

        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::rho}] =
            -eye(Ns,Ns);
        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::p}] =
            diagmat(gas.drhodp(T,p));
        jacrows.at(i)[{_id,gp*nvars_per_gp+constants::T}] =
            diagmat(gas.drhodT(T,p));
           
        *residual.at(i) = st; 

        i++;
    }

    // Equation of state for the last node
    st = gas.rho(Tp,pp) - rhop;
    *residual.at(i) = st; 

    jacrows.at(i)[{_id,(ngp()-1)*nvars_per_gp+constants::rho}] =
        -eye(Ns,Ns);
    jacrows.at(i)[{_id,(ngp()-1)*nvars_per_gp+constants::p}] =
        diagmat(gas.drhodp(T,p));
    jacrows.at(i)[{_id,(ngp()-1)*nvars_per_gp+constants::T}] =
        diagmat(gas.drhodT(T,p));

    i++;

    // Two more equations for the last grid point in case
    // the heat transfer model is not a transport equation.
    if(_ductpb.htmodel() == pb::Isentropic) {

        d T0 = gas.T0();
        d p0 = gas.p0();
        d rho0 = gas.rho0();
        d gamma0 = gas.gamma(T0,p0);

        en = p/p0 - pow(rho/rho0,gamma0);

        jacrows.at(i)[{_id,(ngp()-1)*nvars_per_gp+constants::p}] =
            eye(Ns,Ns)/p0;
        jacrows.at(i)[{_id,(ngp()-1)*nvars_per_gp+constants::rho}] =
            -(gamma0/rho0)*diagmat(pow(rho/rho0,gamma0-1));

        *residual.at(i) = en; i++;
    }

}
PosId Duct::getDof(int varnr,int gp) const {
    // Wraparound
    if(gp<0) gp+=ngp();

    // rho,u,T,p
    // Ts maybe
    us nvars_per_gp = (_ductpb.stempmodel()
                       == pb::HeatBalance ? 5 : 4);


    return {_id,nvars_per_gp*gp+varnr};
}

vd Duct::getvart(int varnr,int gp) const {
    TRACE(14,"Duct::getvart()");
    const SegPositionMapper& sol = sys.getSolution(_id);

    us Ns = sys.Ns();

    // Wraparound
    if(gp<0) gp+=ngp();

    // rho,u,T,p
    // Ts maybe
    us nvars_per_gp = (_ductpb.stempmodel()
                       == pb::HeatBalance ? 5 : 4);
    
    return *sol.at((gp*nvars_per_gp+varnr));
}
vd Duct::getvarx(int varnr,int t) const {
    vd res(ngp());
    for(us i=0;i<ngp();i++){
        res(i) = getvart(varnr,i)(t);
    }
    return res;
}
void Duct::initialSolution(SegPositionMapper& sol) const {
    
    TRACE(15,"Duct::initialSolution()");

    VARTRACE(15,sol.size());

    us nvars_per_gp = 4;
    nvars_per_gp+= (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);

    const Gas& gas = sys.gas();

    us segdof = 0;

    for(us i=0;i<ngp();i++){
        VARTRACE(15,i);
        // Initial density
        *sol.at(segdof) += gas.rho0()+0.1;
        segdof++;
        
        // Initial velocity
        *sol.at(segdof) += 0.1;
        segdof++;

        // Initial Temperature
        *sol.at(segdof) += _Tsprescribed(i);
        segdof++;

        // Initial pressure
        *sol.at(segdof) += gas.p0();
        segdof++;

        // Initial solid temperature, if not prescribed
        if(_ductpb.stempmodel() != pb::Prescribed) {
            *sol.at(segdof) += _Tsprescribed(i);
            segdof++;
        }
    }

}

us Duct::getNEqs() const {
    TRACE(15,"Duct::getNEqs()");    

    // The number of equations per gridpoint. We have: continuity,
    // momentum, energy, and state
    us number_eqs = 4;

    // When we have to solve a solid heat balance
    number_eqs+= (_ductpb.stempmodel() == pb::HeatBalance ? 1 : 0);

    us neqs = number_eqs*(ngp()-1);

    // For the last gridpoint, we also have an equation of state
    neqs += 1;

    // We also have an extra equation for isentropic. For the energy
    // transport equation, this would result in a boundary condition
    if(_ductpb.htmodel() == pb::Isentropic) {
        neqs += 1;
    }

    VARTRACE(15,neqs);
    return neqs;
}
us Duct::getNDofs() const {
    TRACE(15,"Duct::getNDofs()");        
 
    // rho,u,T,p
    // Ts maybe
    us nvars_per_gp = (_ductpb.stempmodel()
                       == pb::HeatBalance ? 5 : 4);

    return nvars_per_gp*ngp();
}
d Duct::getMass() const {

    return 0;
}
void Duct::show(us verbosity_level) const {

}

void Duct::exportHDF5(const hid_t group_id) const {

    TRACE(15,"Duct::exportHDF5");

    PosData x;
    x.name = "Position";
    x.unit = "m";
    x.symbol = "x";
    x.x = this->x;
    x.exportHDF5(group_id);

    PosData S;
    S.name = "Cross-sectional area";
    S.unit = "m$^2$";
    S.symbol = "S";
    S.x = this->S;
    S.exportHDF5(group_id);

    PosData phi;
    phi.name = "Porosity";
    phi.unit = "-";
    phi.symbol = "phi";
    phi.x = this->phi;
    phi.exportHDF5(group_id);

    PosData rh;
    rh.name = "Hydraulic radius";
    rh.unit = "m";
    rh.symbol = "rh";
    rh.x = this->rh;
    rh.exportHDF5(group_id);

    TXData u;
    u.name = "Axial velocity";
    u.unit = "m/s";
    u.symbol = "u";
    u.x = dmat(sys.Ns(),ngp());
    for(us gp=0;gp<ngp();gp++){
        u.x.col(gp) = ut(gp);
     }
    u.exportHDF5(group_id);

    TXData T;
    T.name = "Fluid temperature";
    T.unit = "K";
    T.symbol = "T";
    T.x = dmat(sys.Ns(),ngp());
    for(us gp=0;gp<ngp();gp++){
        T.x.col(gp) = Tt(gp);
    }
    T.exportHDF5(group_id);

    TXData p;
    p.name = "Fluid pressure";
    p.unit = "Pa";
    p.symbol = "p";
    p.x = dmat(sys.Ns(),ngp());
    for(us gp=0;gp<ngp();gp++){
        p.x.col(gp) = pt(gp);
    }
    p.exportHDF5(group_id);

    // TXData Ts;
    // Ts.name = "Solid temperature";
    // Ts.unit = "K";
    // Ts.symbol = "Ts";
    // Ts.x = dmat(sys.Ns(),ngp());
    // for(us gp=0;gp<ngp();gp++){
    //     p.x.col(gp) = Tst(gp);
    // }
    // Ts.exportHDF5(group_id);
    

}
//////////////////////////////////////////////////////////////////////
