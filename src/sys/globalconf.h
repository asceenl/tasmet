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

#include "tasmet_types.h"
#include "tasmet_tracer.h"

class GlobalConf{
    d _omg;		// The "base" frequency in rad/s
    us _Nf;			// Number of frequencies to solve for

    dmat fDFT_,iDFT_,DDTfd_;
    // d Wfo_=0;			// First order 'upwind' factor. If
    // Wfo=-1, interpolation is done from
    // the left side. If Wfo=0,
    // interpolation is second order. If
    // Wfo=1, interpolation is done from
    // the right side
public:
    GlobalConf(us Nf,d freq);
    
    us Nf() const {return _Nf;}
    us Ns() const {return 2*_Nf+1;}    

    ~GlobalConf(){TRACE(-5,"~GlobalConf()");}
    d getomg() const {return _omg;}
    d getfreq() const {return _omg/2/number_pi;}
    // d meshPeclet(const Gas& gas,d dx,d u) const {return u*dx*gas.rho0()*gas().cp(T0())/gas().kappa(T0());}

    void set(us Nf,d freq);	// Set data for new frequency and
    // number of samples

    void setNf(us Nf){set(Nf,getfreq());}  
    void setfreq(d freq){set(Nf(),freq);}
    void setomg(d omg)  {set(Nf(),omg/(2*number_pi));}

    const dmat& iDFT() const {return iDFT_;} //inverse discrete Fourier transform matrix
    const dmat& fDFT() const {return fDFT_;} //forward discrete Fourier transform matrix
    const dmat& DDTfd() const {return DDTfd_;}//Derivative in frequency domain

    void show() const;

}; /* Class GlobalConf */

#endif /* _GLOBALCONF_H_ */
//////////////////////////////////////////////////////////////////////
