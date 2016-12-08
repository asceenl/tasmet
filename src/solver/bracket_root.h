// bracket_lin.h
//
// Author: J.A. de Jong 
//
// Description: Bracket a function's root given a guess value, returns
// another guess where the solution has the opposite sign
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BRACKET_LIN_H
#define BRACKET_LIN_H
#include "system.h"
#include "tasmet_tracer.h"
#include <limits>

inline int sign(d& x){
    return x < 0? -1: 1;
}
const us maxiter = 100;

inline std::pair<d,d> bracket_root(NoGradientNonlinearSystem<d>& sys,const d guess) {

    TRACE(10,"bracket_root()");

    d xa = guess;
    d fa = sys.residual(xa);

    if(fa == 0) {
        // Guess was exactly right
        return std::make_pair(xa,xa);
    }
    
    d fb = fa;

    // We add here a small amount, such that, in case the first guess
    // is 0, we 
    d xb = xa+std::numeric_limits<d>::epsilon();

    d factor = 1.001;
    d delta = xa/10 + std::numeric_limits<d>::epsilon();
    
    us iter = 0;

    int step = 64;

    bool switched = false;
    
    while (iter < maxiter) {

        VARTRACE(5,xb);
        
        xb += delta*factor;

        fb = sys.residual(xb);

        if(sign(fa) != sign(fb)) {
            // Found it!
            VARTRACE(5,xa);
            VARTRACE(5,xb);
            VARTRACE(5,fa);
            VARTRACE(5,fb);
            return std::make_pair(xa,xb);
        }
        
        if (abs(fb) < abs(fa)){
            // We found a point which is closer to the real root
            xa = xb;
            fa = fb;
        }
        else if(!switched) {
            TRACE(5,"Switching search direction");
            switched = true;
            // We searched in the wrong direction
            factor*=-1;
        }

        // Heuristic: normally it's best not to increase the step sizes as we'll just end up
        // with a really wide range to search for the root.  However, if the initial guess was *really*
        // bad then we need to speed up the search otherwise we'll take forever if we're orders of
        // magnitude out.  This happens most often if the guess is a small value (say 1) and the result
        // we're looking for is close to std::numeric_limits<T>::min().
        //       

        if((10000 - iter) % step == 0) {
            factor *=2;
            if(step > 1) step/=2;
        }

        iter++;
    }

    WARN("Failed to bracket root");

    return std::make_pair(xa,xb);
    
}
#endif // BRACKET_LIN_H
//////////////////////////////////////////////////////////////////////
