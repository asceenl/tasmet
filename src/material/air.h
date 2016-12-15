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
    vdi<5> _cpc = {1047.63657,-0.372589265,     \
                   9.45304214E-4,-6.02409443E-7,        \
                   1.2858961E-10};
    
    vdi<5> _kappac = {-0.00227583562,1.15480022E-4,     \
                      -7.90252856E-8,4.11702505E-11,    \
                      -7.43864331E-15};

    vdi<5> _muc = {-8.38278E-7,8.35717342E-8,   \
                   -7.69429583E-11,4.6437266E-14,       \
                   -1.06585607E-17};
protected:
    d Rs() const {return 287;}
public:
    Air(d T0,d p0):PerfectGas(air,T0,p0){}
    const vd& cpc() const {return _cpc;}
    d mu(d T,d p) const;
    d kappa(d T,d p) const;
    ~Air(){}

    Air* copy() const { return new Air(T0(),p0());}
};

  


#endif // AIR_H
//////////////////////////////////////////////////////////////////////
