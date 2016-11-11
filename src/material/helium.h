// helium.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef HELIUM_H
#define HELIUM_H
#include "perfectgas.h"

class Helium :public PerfectGas {
protected:
    d Rs() const {return 2070;}
public:
    Helium():PerfectGas(helium){}

    d cp(d T,d p) const { return 5195;}
    vd cp(const vd& T,const vd& p) const;

    d h(d T,d p) const {return cp(0.0,0.0)*T;}
    vd h(const vd& T,const vd& p) const;

    d mu(d T,d p) const;
    vd mu(const vd& T,const vd& p) const;

    d kappa(d T,d p) const;
    vd kappa(const vd& T,const vd& p) const;

    virtual ~Helium(){}
};

  



#endif // HELIUM_H
//////////////////////////////////////////////////////////////////////
