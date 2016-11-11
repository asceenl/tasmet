// rottfuncs.h
//
// Author: J.A. de Jong 
//
// Description:
// Implementation of thermoviscous (Rott's) functions
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ROTTFUNCS_H
#define ROTTFUNCS_H
#include "tasmet_types.h"
#include "tasmet_tracer.h"
#include "tasmet_enum.h"

DECLARE_ENUM(cshape,INVISCID,BLAPPROX,VERT,SQUARE,CIRC)

class RottFuncs {		// Thermoviscous Rott functions
    vc (*f_ptr)(const vc& rh_over_delta);
    c (*f_ptrc)(const c& rh_over_delta);
    cshape _shape;
public:
    RottFuncs(const cshape shape=cshape::INVISCID);
    RottFuncs(const RottFuncs& other) =delete;
    RottFuncs& operator=(const RottFuncs&) =delete;
    ~RottFuncs(){}

    vc operator()(const vc& rh_over_delta) const {
        TRACE(5,"fx vc");
        return f_ptr(rh_over_delta);
    }
    vc operator()(const vd& rh_over_delta) const {
        TRACE(5,"fx vd");
        return f_ptr((1.0+0.0*I)*rh_over_delta);
    }

    c operator()(const c& rh_over_delta) const {return f_ptrc(rh_over_delta);}
    c operator()(const d& rh_over_delta) const {c n(rh_over_delta,0); return f_ptrc(n);}
    cshape getCshape() const {return _shape;}
};


#endif // ROTTFUNCS_H
//////////////////////////////////////////////////////////////////////
