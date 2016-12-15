// nitrogen.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef NITROGEN_H
#define NITROGEN_H

#include "perfectgas.h"

  
class Nitrogen : public PerfectGas {
    vdi<5> _cpc={3.29868,0.00140824,                
                 -3.96322e-6,
                 5.64152e-9,            
                 -2.44486e-12};
    
    vdi<2> _kappac={6.995e-3,0.0631e-3};

protected:
    d Rs() const {return 297;}
public:
    Nitrogen(d T0,d p0):PerfectGas(nitrogen,T0,p0){}
    Nitrogen* copy() const { return new Nitrogen(T0(),p0());}  
    const vd& cpc() const { return _cpc; }
    d mu(d T,d p) const;
    d kappa(d T,d p) const;

};

  

#endif // NITROGEN_H

//////////////////////////////////////////////////////////////////////
