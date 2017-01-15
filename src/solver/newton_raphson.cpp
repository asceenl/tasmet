// newton_raphson.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "newton_raphson.h"
#include "tasmet_tracer.h"

#define DEBUG_TASMET_SYSTEM

void NewtonRaphson::start_implementation(GradientNonlinearSystem& system,
                                         progress_callback* callback) {

    assert(callback);

    us niter = 0;

    // Relative error, function error
    d reler,funer;

    vd guess = system.getSolution();

    vd residual=system.residual();

    #ifdef DEBUG_TASMET_SYSTEM
    cout << "Initial solution: " << endl;
    cout << guess << endl;
    #endif  // DEBUG_TASMET_SYSTEM
    
    #ifdef DEBUG_TASMET_SYSTEM
    cout << "Initial residual: " << endl;
    cout << residual << endl;
    #endif  // DEBUG_TASMET_SYSTEM


    SolverProgress progress;
    SolverAction action;
    
    while (true) {
        
        sdmat jac=system.jacobian();

        assert(jac.n_cols==residual.size());
        assert(jac.n_rows==residual.size());

        vd dx = -1*_dampfac*arma::spsolve(jac,residual,"superlu");

        guess += dx;

        system.updateSolution(guess);

        residual = system.residual();
        #ifdef DEBUG_TASMET_SYSTEM
        cout << "Residual: ";
        cout << residual << endl;
        #endif  // DEBUG_TASMET_SYSTEM

    
        progress.rel_err = norm(dx);
        progress.fun_err = norm(residual);
        progress.iteration++;

        action = (*callback)(progress);

        if(action==Stop){
            break;
        }
            
    }

}
//////////////////////////////////////////////////////////////////////
