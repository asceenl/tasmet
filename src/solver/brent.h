// brent.h
//
// Author: J.A. de Jong 
//
// Description:
// Brent's root finding algorithm
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BRENT_H
#define BRENT_H
#include "solver.h"

class Brent: public Solver<NoGradientNonlinearSystem<d>,d> {
    d _reltol;
    us _maxiter;
public:    
    Brent(const NoGradientNonlinearSystem<d>& sys,us maxiter=10000,d reltol=1e-6);
protected:
    void start_implementation(NoGradientNonlinearSystem<d>& sys,progress_callback*);
};


#endif // BRENT_H
//////////////////////////////////////////////////////////////////////
