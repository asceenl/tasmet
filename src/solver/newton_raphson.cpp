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

    #ifdef DEBUG_TASMET_SYSTEM
    cout << "Initial solution: " << endl;
    cout << guess << endl;
    #endif  // DEBUG_TASMET_SYSTEM
    
    SolverProgress progress;
    SolverAction action;
    
    ResidualJac resjac;
    system.residualJac(resjac);

    vd dx;

    assert(resjac.jacobian.n_cols==resjac.residual.size());
    assert(resjac.jacobian.n_rows==resjac.residual.size());

    while (true) {

        #ifdef DEBUG_TASMET_SYSTEM
        cout << "Residual: ";
        cout << resjac.residual << endl;
        cout << dmat(resjac.jacobian) << endl;        
        #endif  // DEBUG_TASMET_SYSTEM
    
        TRACE(15,"Solving system of eqs");
        dx = -1*_dampfac*arma::spsolve(resjac.jacobian,resjac.residual,"superlu");
        TRACE(15,"Solving system of eqs done");
        
        progress.rel_err = norm(dx);

        guess += dx;

        system.updateSolution(guess);

        // Obtain a new residual and Jacobian matrix
        system.residualJac(resjac);

        progress.fun_err = norm(resjac.residual);
        progress.iteration++;

        action = (*callback)(progress);

        if(action==Stop){
            TRACE(15,"Solution at stop:");
            TRACE(15,guess);
            break;
        }
            
    }

}
//////////////////////////////////////////////////////////////////////
