// newton_raphson_test.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Test program to test the Newton-Raphson solver implementation
//////////////////////////////////////////////////////////////////////
#include "system.h"
#include "brent.h"
#include "tasmet_io.h"
#include "tasmet_tracer.h"
#include "helium.h"
#include "nitrogen.h"
#include "air.h"
#include "globalconf.h"
#include "tasmet_variable.h"
#include "adiabatictemp.h"

SolverAction solver_callback(SolverProgress p){

    cout << "Iteration: " << p.iteration << ". Function error: " << p.fun_err <<". Relative error: " << p.rel_err << endl;
    if(p.fun_err < 1e-7){
        TRACE(9, "Returning stop");
        return Stop;
    }
    else {
        TRACE(9, "Returning continue");
        return Continue;
    }
}

class TestFunction: public  NoGradientNonlinearSystem<d> {
    d cur_sol;
public:
    TestFunction(d guess){cur_sol = guess;}    
    d residual() const {
        return pow(cur_sol,2)-3;
    }

    void updateSolution(const d& res){ cur_sol = res;}
    d getSolution() const { return cur_sol;}
    TestFunction* copy() const {return new TestFunction(cur_sol);}
    sdmat jacobian() const {
        sdmat res(1,1);
        res(0,0) = 2*cur_sol;
        return res;
    }
};

int main(){

    INITTRACE(5);
    
    TestFunction t(100);
    Brent solver(t);
    
    std::function<SolverAction(SolverProgress)> p = solver_callback;
    solver.start(&p);

    d res = solver.getSolution();
    
    cout << "Final solution: x = " << res << endl;

    Nitrogen nit(293.15,101325);
    Air air(293.15,101325);
    Helium helium(293.15,101325);

    gc_ptr gc(new GlobalConf(10,100));
    Variable pressure(gc,10*101325);
    Variable temperature = adiabaticTemp(helium,pressure);
}



//////////////////////////////////////////////////////////////////////
