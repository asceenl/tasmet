// system.h
//
// Author: J.A. de Jong 
//
// Description:
// This header file describes an interface to a system of equations
// that can be solved using a (non)linear solver.
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H
#include "tasmet_types.h"
#include "tasmet_assert.h"

template<typename T>
class NoGradientNonlinearSystem{
    // A nonlinear system for which the gradient of the residual to
    // the solution vector cannot be obtained.
public:
    virtual T residual() const=0;

    virtual T residual(const T& guess) {
        updateSolution(guess);
        return residual();
    }
    
    // Obtain an initial guess of the solution
    virtual T getSolution() const=0;

    // Create a copy of the system
    virtual NoGradientNonlinearSystem* copy() const=0;

    virtual void updateSolution(const T& new_guess)=0;

    virtual ~NoGradientNonlinearSystem(){}
};


// Wrapper around a vector-valued system, such that a solver for a
// multi-dimensional system can be called for a single-DOF system
template <typename T>
class SingleDofNoGradient: public NoGradientNonlinearSystem<vd> {

    static_assert(is_same<T,d>::value || is_same<T,c>::value,"Instaniation must be complex or double");

    NoGradientNonlinearSystem<T>* wrapped_system = nullptr;
    
public:
    
    SingleDofNoGradient(const NoGradientNonlinearSystem<T>& wrapped_system);
    
    virtual vd residual() const;

    virtual vd getSolution() const;

    // Create a copy of the system
    virtual SingleDofNoGradient<T>* copy() const;

    virtual void updateSolution(const vd& new_guess);

    ~SingleDofNoGradient();

};

struct ResidualJac {
    vd residual;                // Store residual here
    sdmat jacobian;             // Store Jacobian here
};

class GradientNonlinearSystem {

public:
    /** 
     * Computes both the residual as well as the Jacobian, and stores
     * them in the input parameter
     *
     */
    virtual void residualJac(ResidualJac&) const=0;

    virtual GradientNonlinearSystem* copy() const=0;

    // Obtain an initial guess of the solution
    virtual vd getSolution() const=0;

    virtual void updateSolution(const vd& new_guess)=0;
    virtual ~GradientNonlinearSystem(){}
};



#endif // SYSTEM_H
//////////////////////////////////////////////////////////////////////
