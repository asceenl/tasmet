// brent.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Brent's root finding algorithm as implemented from Wikipedia:
// https://en.wikipedia.org/wiki/Brent's_method
//////////////////////////////////////////////////////////////////////
#define TRACERPLUS (10)
#include "brent.h"
#include "tasmet_tracer.h"
#include "tasmet_exception.h"
#include <limits>
#include "tasmet_constants.h"
#include <algorithm>            // std::swap
#include "bracket_root.h"

const d eps = std::numeric_limits<d>::epsilon();

Brent::Brent(const NoGradientNonlinearSystem<d>& sys,d reltol):
    Solver(sys),
    _reltol(reltol)
{

    TRACE(15,"Brent::Brent");
    #ifdef TASMET_DEBUG
    bool ok=true;

    ok&=(reltol >= eps);

    if(!ok){
        throw TaSMETError("Brent: invalid arguments");
    }
    #endif
}
namespace {
    inline bool is_between(d x,d a,d b) {
        const d& mi = std::min(a,b);
        const d& ma = std::max(a,b);
        return ((mi <= x) && (x <=ma));
    }
}

void Brent::start_implementation(NoGradientNonlinearSystem<d>& system,
                                 progress_callback* callback) {

    TRACE(15,"Brent::start_implementation");

    us niter = 0;

    d a = system.getSolution();

    std::pair<d,d> brackets = bracket_root(system,a);
    a = brackets.first;
    d b = brackets.second;

    d fa = system.residual(a);    
    if((fa) == 0) {
        TRACE(15,"Found root during bracketing");
        return;
    }
    
    d fb = system.residual(b);
    if((fb) == 0) {
        TRACE(15,"Found root during bracketing");
        return;
    }
    d c = a;
    d fc = fa;

    d s;                        // Test point
    d fs;                       // Function at test point

    d t,u,v;
    d d_;

    if(fa*fb>=0){
        WARN("Brent solver failed: root is not bracketed");
        return;
    }

    if(abs(fa) < abs(fb)) {
        std::swap(a,b);
        std::swap(fa,fb);
    }

    bool mflag = true;
    bool bisec_flag;
    SolverProgress progress;

    while(true) {

        if((fa!=fc) && (fb!=fc)){
            // Inverse quadratic interpolation
            TRACE(5,"IQI");
            t = a*fb*fc/((fa-fb)*(fa-fc));
            u = b*fa*fc/((fb-fa)*(fb-fc));
            v = c*fa*fb/((fc-fa)*(fc-fb));

            s = t+u+v;
        }
        else {
            // Secant method
            TRACE(5,"Secant");
            s = b-fb*(b-a)/(fb-fa);
        }

        bisec_flag = false;

        d abssmb = abs(s-b);
        d absbmc = abs(b-c);
        d abscmd = abs(c-d_);
        
        VARTRACE(5,s);

        if((bisec_flag |= (!is_between(s,(3*a+b)/4,b)))) goto bflag;
        TRACE(5,"Survived 1");
        if(bisec_flag |= (mflag && (abssmb >= absbmc/2))) goto bflag;
        TRACE(5,"Survived 2");
        if(bisec_flag |= ((!mflag) && (abssmb >= abscmd/2))) goto bflag;
        TRACE(5,"Survived 3");
        if(bisec_flag |= (mflag && (absbmc < abs(_reltol)))) goto bflag;;
        TRACE(5,"Survived 4");
    bflag:
        if(bisec_flag || ((!mflag) && (abscmd < abs(_reltol)))) {
            TRACE(5,"Bisection");
            s = (a+b)/2;
            mflag = true;
        }
        else {
            mflag = false;
        }

        fs = system.residual(s);
        d_ = c;
        
        if(fa*fs < 0) {
            c = b;
            fc = fb;
            b=s;
            fb = fs;
        }
        else {
            c = a;
            fc = fa;
            a = s;
            fa = fs;
        }
        if(abs(fa)<abs(fb)) {
            std::swap(a,b);
            std::swap(fa,fb);            
        }
        
        progress.fun_err = abs(fs);
        progress.rel_err = abs(b-a);
        progress.iteration++;

        VARTRACE(5,s);
        VARTRACE(5,a);
        VARTRACE(5,b); 
        VARTRACE(5,c);
        VARTRACE(5,fa);
        VARTRACE(5,fb); 
        VARTRACE(5,fc);
        VARTRACE(5,fs);

        SolverAction action = (*callback)(progress);

        if(action==Stop){
            break;
        }


    }
    
    system.updateSolution(b);
}


//////////////////////////////////////////////////////////////////////
