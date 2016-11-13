// air.h
//
// Author: J.A. de Jong 
//
// Description:
// Implementation of the gas air
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AIR_H
#define AIR_H
#include "perfectgas.h"


class Air : public PerfectGas {
protected:
    d Rs() const {return 287;}
public:
    Air(d T0,d p0):PerfectGas(air,T0,p0){}
    d cp(d T,d p) const;
    vd cp(const vd& T,const vd& p) const;
    d h(d T,d p) const;
    vd h(const vd& T,const vd& p) const;
    d mu(d T,d p) const;
    vd mu(const vd& T,const vd& p) const;
    d kappa(d T,d p) const;
    vd kappa(const vd& T,const vd& p) const;
    ~Air(){}
};

  


#endif // AIR_H
//////////////////////////////////////////////////////////////////////
