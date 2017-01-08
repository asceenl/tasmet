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

/**
 * Newton Raphson Solver implementation.
 * 
 */
class NewtonRaphson: public Solver<GradientNonlinearSystem,vd> {

    d _dampfac = 1.0; /**< This is the applied damping factor */
public:
    NewtonRaphson(const GradientNonlinearSystem&sys,d dampfac=1.0):
        Solver(sys),
        _dampfac(dampfac)
        {}
protected:
    void start_implementation(GradientNonlinearSystem& system,progress_callback* callback);
    
};


#endif // NEWTON_RAPHSON_H
//////////////////////////////////////////////////////////////////////
