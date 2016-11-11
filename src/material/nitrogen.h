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
protected:
    d Rs() const {return 297;}
public:
    Nitrogen():PerfectGas(nitrogen){}

    d cp(d T,d p) const;
    vd cp(const vd& T,const vd& p) const;
    d h(d T,d p) const;
    vd h(const vd& T,const vd& p) const;
    d mu(d T,d p) const;
    vd mu(const vd& T,const vd& p) const;
    d kappa(d T,d p) const;
    vd kappa(const vd& T,const vd& p) const;
    ~Nitrogen(){}
};

  

#endif // NITROGEN_H

//////////////////////////////////////////////////////////////////////
