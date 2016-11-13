// perfectgas.h
//
// Author: J.A. de Jong 
//
// Description:
//
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

    virtual d Rs() const=0;

public:
    ~PerfectGas(){}

    vd rho(const vd&T,const vd&p) const {
        checkzero(T);
        return p/Rs()/T;
    }
    vd p(const vd& T,const vd& rho) const {
        return rho%(Rs()*T);
    }
    vd cv(const vd& T,const vd& p) const {
        return cp(T,p)-Rs();
    }
    vd e(const vd& T,const vd& p) const {
        return h(T,p)-Rs()*T;
    }
    d beta(d T,d p) const {
        checkzero(T);
        return 1/T;
    }
    vd beta(const vd& T,const vd& p) const {
        return 1/T;
    }
    vd cm(const vd& T,const vd& p) const {
        return sqrt(gamma(T,p)*Rs()%T);
    }
    d rho(d T,d p) const {
        checkzero(T);
        return p/Rs()/T;
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

