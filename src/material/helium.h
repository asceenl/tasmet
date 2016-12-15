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
    vdi<1> _cpc = {5195};
protected:
    d Rs() const {return 2070;}
public:
    Helium(d T0,d p0):PerfectGas(helium,T0,p0){}

    const vd& cpc() const { return _cpc; }
    d cp(d T,d p) const { return _cpc(0);}

    d mu(d T,d p) const;

    d kappa(d T,d p) const;
    Helium* copy() const { return new Helium(T0(),p0());}

    virtual ~Helium(){}
};

  



#endif // HELIUM_H
//////////////////////////////////////////////////////////////////////
