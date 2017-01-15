// perfectgas.h
//
// Author: J.A. de Jong 
//
// Description: A thermally perfect gas obeys the ideal-gas law
// equation of state (p=rho*R*T) but has TEMPERATURE-DEPENDENT and
// PRESSURE-DEPENDENT specific heat constants. Our definition of a
// perfect gas has only temperature-dependent specific heat, viscosity
// and thermal conductivity values.
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PERFECTGAS_H
#define PERFECTGAS_H
#include "gas.h"
#ifdef DEBUG_GAS
#define checkzero(x)                                    \
    TRACE(-1,"Divide by zero testing entered.");	\
    try {if(min(abs(x))<1e-13) throw 0; }               \
    catch(int a){					\
        TRACE(0,"Divide by zero encountered.");         \
    }
#else
#define checkzero(x)
#endif

  
class PerfectGas: public Gas {
protected:
    PerfectGas(GasType gastype,d T0,d p0): Gas(gastype,T0,p0){}

    // Not implemented for a perfect gas:
    // mu, kappa, h, cp, Rs

public:
    virtual d Rs() const=0;

    // Vector of polynomial coefficients to compute the
    // specific heat at constant pressure
    virtual const vd& cpc() const=0;
    
    ~PerfectGas(){}

    d h(d T,d p) const {
        const vd& cpc_ = this->cpc();
        us nfac = cpc_.size();
        vd powfac(nfac);
        
        for(us i=0;i<nfac;i++)
            powfac(i) = pow(T,i)/(1+i);
        
        return dot(cpc_,powfac);
    }
    d cp(d T,d p) const {
        const vd& cpc_ = this->cpc();
        us nfac = cpc_.size();
        vd powfac(nfac);
        
        for(us i=0;i<nfac;i++)
            powfac(i) = pow(T,i);
        return dot(cpc_,powfac);
    }
    d rho(d T,d p) const {
        checkzero(T);
        return p/Rs()/T;
    }
    d drhodT(d T,d p) const {
        return -p/Rs()/pow(T,2);
    }
    d drhodp(d T,d p) const {
        return Rs()/T;
    }

    d cv(d T,d p) const {
        return cp(T,p)-Rs();
    }
    d e(d T,d p) const {
        return h(T,p)-Rs()*T;
    }
    d cm(d T,d p) const {
        d csq=gamma(T,p)*Rs()*T;
        return sqrt(csq);
    }
    
};


#endif // PERFECTGAS_H
//////////////////////////////////////////////////////////////////////

