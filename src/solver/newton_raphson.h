// newton_raphson.h
//
// Author: J.A. de Jong 
//
// Description:
// Implementation of the Newton-Raphson method
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef NEWTON_RAPHSON_H
#define NEWTON_RAPHSON_H
#include "solver.h"

class NewtonRaphson: public Solver<GradientNonlinearSystem,vd> {

    d _dampfac = 1.0;
    d _maxiter = 10000;
public:
    NewtonRaphson(const GradientNonlinearSystem&sys,d dampfac,us maxiter):
        Solver(sys),
        _dampfac(dampfac),
        _maxiter(maxiter){}

protected:
    void start_implementation(GradientNonlinearSystem& system,progress_callback* callback);
    
};


#endif // NEWTON_RAPHSON_H
//////////////////////////////////////////////////////////////////////
