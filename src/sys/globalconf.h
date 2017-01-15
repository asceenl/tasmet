// globalconf.h
//
// Author: J.A. de Jong 
//
// Description:
// Global configuration options
//////////////////////////////////////////////////////////////////////
#pragma once

#ifndef _GLOBALCONF_H_
#define _GLOBALCONF_H_

#include <memory>
#include "tasmet_types.h"
#include "tasmet_tracer.h"

class GlobalConf;

typedef std::shared_ptr<GlobalConf> gc_ptr;

class GlobalConf{
    d _omg;		// The "base" frequency in rad/s
    us _Nf;			// Number of frequencies to solve for

    dmat _fDFT,_iDFT,_DDTfd;
    // d Wfo_=0;			// First order 'upwind' factor. If
    // Wfo=-1, interpolation is done from
    // the left side. If Wfo=0,
    // interpolation is second order. If
    // Wfo=1, interpolation is done from
    // the right side
public:
    GlobalConf(us Nf,d freq);

    const dmat& iDFT = _iDFT; //inverse discrete Fourier transform matrix
    const dmat& fDFT =  _fDFT; //forward discrete Fourier transform matrix
    const dmat& DDTfd = _DDTfd; //Derivative in frequency domain
    
    us Nf() const {return _Nf;}
    us Ns() const {return 2*_Nf+1;}    

    ~GlobalConf(){TRACE(-5,"~GlobalConf()");}
    d getomg() const {return _omg;}
    d getfreq() const {return _omg/2/number_pi;}
    // d meshPeclet(const Gas& gas,d dx,d u) const {return u*dx*gas.rho0()*gas().cp(T0())/gas().kappa(T0());}

    vd timeInstances() const;

    void setfreq(d freq){setomg(2*number_pi*freq);}
    void setomg(d omg);

    void show() const;
    
}; /* Class GlobalConf */

#endif /* _GLOBALCONF_H_ */
//////////////////////////////////////////////////////////////////////
