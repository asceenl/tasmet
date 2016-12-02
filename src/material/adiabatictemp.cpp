// adiabatictemp.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#define TRACERPLUS 10
#include "adiabatictemp.h"
#include "perfectgas.h"
#include "tasmet_variable.h"
#include "tasmet_tracer.h"
#include "tasmet_exception.h"
#include "brent.h"

struct AdiabaticTemp : public NoGradientNonlinearSystem<d> 
{
    d p,p0,Rs,T0;
    d T;
    const PerfectGas& gas;      // Careful here, reference!
    const vd& cpc;              // Here too!

    d gamma0;                   // Used for initial guess

    AdiabaticTemp(const PerfectGas& gas,d p):
        gas(gas),
        cpc(gas.cpc())
    {

        p0 = gas.p0();
        T0 = gas.T0();
        Rs = gas.Rs();

        gamma0 = gas.gamma(T0,p0);

        #ifdef TASMET_DEBUG
        if(cpc.size() < 1)
            throw TaSMETError("Invalid array of heat capacity coefficients");
        #endif

        setP(p);
        
    }

    d getSolution() const { return T; }
    void updateSolution(const d& T) { this->T=T;}

    AdiabaticTemp* copy() const { return new AdiabaticTemp(gas,p);}
    
    // Set a pressure to solve the temperature for
    void setP(d p) {

        this->p = p;
        
        // Good initial guess
        T = T0*pow(p/p0,(gamma0-1)/gamma0);

    }
    


    // Final function to solve for
    d residual() const {
    
        // Do the integration for the temperature
        d lhs = (cpc(0)/Rs)*log(T/T0);
        for(us i=1;i<cpc.size();i++){
            lhs+= cpc(i)*(pow(T,d(i))-pow(T0,d(i)))/(i*Rs);
        }

        d rhs = log(p/p0);

        return lhs-rhs;
    }

};
namespace {
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
}
Variable adiabaticTemp(const PerfectGas& gas,
                       const Variable& pressure) {
                       
    
    TRACE(10,"adiabaticTemp()");
    Variable temp(pressure);
    const vd p = pressure.tdata();

    vd T = temp.tdata();

    AdiabaticTemp adt(gas,p(0)); // Equation

    std::function<SolverAction(SolverProgress)> cb = solver_callback;

    for(us i=0;i<p.size();i++){

        adt.setP(p(i));
        Brent brent(adt);           // Solver

        VARTRACE(15,p(i));
        VARTRACE(15,adt.getSolution());
        brent.start(&cb);

        T(i) = brent.getSolution();
        VARTRACE(15,T(i));

    }
    temp.settdata(T);
    return temp;
}
//////////////////////////////////////////////////////////////////////

