// newton_raphson_test.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Test program to test the Newton-Raphson solver implementation
//////////////////////////////////////////////////////////////////////
#include "system.h"
#include "newton_raphson.h"
#include "tasmet_io.h"
#include "tasmet_tracer.h"

SolverAction solver_callback(SolverProgress p){

    cout << "Iteration: " << p.iteration << ". Function error: " << p.fun_err << endl;
    if(p.fun_err < 1e-7){
        TRACE(9, "Returning stop");
        return Stop;
    }
    else {
        TRACE(9, "Returning continue");
        return Continue;
    }
}

class TestFunction: public  GradientNonlinearSystem {
    d cur_sol;
public:
    TestFunction(d guess){cur_sol = guess;}    
    vd residual() const {
        d val = pow(cur_sol,2)-3;
        return vd({val});
    }

    void updateSolution(const vd& res){ cur_sol = res(0);}
    vd getSolution() const { return vd({cur_sol});}
    TestFunction* copy() const {return new TestFunction(cur_sol);}
    sdmat jacobian() const {
        sdmat res(1,1);
        res(0,0) = 2*cur_sol;
        return res;
    }
};

int main(){

    INITTRACE(10);
    
    TestFunction t(100);
    NewtonRaphson solver(t,1.0,1000);
    
    std::function<SolverAction(SolverProgress)> p = solver_callback;
    solver.start(&p);
    solver.stop();
    
    cout << "Final solution: x = " << solver.getSolution();

}



//////////////////////////////////////////////////////////////////////
