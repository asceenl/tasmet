// broyden.h
//
// Author: J.A. de Jong 
//
// Description: Implementation of the Broyden solver, a gradient-free
// nonlinear system solver
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BROYDEN_H
#define BROYDEN_H
#include "solver.h"



class Broyden: public Solver<NoGradientNonlinearSystem<vd> >{

public:

};


#endif // BROYDEN_H
//////////////////////////////////////////////////////////////////////
