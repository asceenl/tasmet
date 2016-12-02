// brent.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Brent's root finding algorithm as implemented from Wikipedia:
// https://en.wikipedia.org/wiki/Brent's_method
//////////////////////////////////////////////////////////////////////

#include "brent.h"
#include "tasmet_tracer.h"
#include "tasmet_exception.h"
#include <limits>
#include "tasmet_constants.h"
#include <algorithm>            // std::swap
#include "bracket_root.h"

const d eps = std::numeric_limits<d>::epsilon();

Brent::Brent(const NoGradientNonlinearSystem<d>& sys,us maxiter,d reltol):
    Solver(sys),
    _reltol(reltol),
    _maxiter(maxiter)
{

    TRACE(15,"Brent::Brent");
    #ifdef TASMET_DEBUG
    bool ok=true;
    ok&=(maxiter>0);
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
    d fb = system.residual(b);

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

    while(_running && progress.iteration <=_maxiter) {

        if((fa!=fc) && (fb!=fc)){
            // Inverse quadratic interpolation
            TRACE(15,"IQI");
            t = a*fb*fc/((fa-fb)*(fa-fc));
            u = b*fa*fc/((fb-fa)*(fb-fc));
            v = c*fa*fb/((fc-fa)*(fc-fb));

            s = t+u+v;
        }
        else {
            // Secant method
            TRACE(15,"Secant");
            s = b-fb*(b-a)/(fb-fa);
        }

        bisec_flag = false;

        d abssmb = abs(s-b);
        d absbmc = abs(b-c);
        d abscmd = abs(c-d_);
        
        VARTRACE(15,s);

        if((bisec_flag |= (!is_between(s,(3*a+b)/4,b)))) goto bflag;
        TRACE(15,"Survived 1");
        if(bisec_flag |= (mflag && (abssmb >= absbmc/2))) goto bflag;
        TRACE(15,"Survived 2");
        if(bisec_flag |= ((!mflag) && (abssmb >= abscmd/2))) goto bflag;
        TRACE(15,"Survived 3");
        if(bisec_flag |= (mflag && (absbmc < abs(_reltol)))) goto bflag;;
        TRACE(15,"Survived 4");
    bflag:
        if(bisec_flag || ((!mflag) && (abscmd < abs(_reltol)))) {
            TRACE(15,"Bisection");
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

        VARTRACE(15,s);
        VARTRACE(15,a);
        VARTRACE(15,b); 
        VARTRACE(15,c);
        VARTRACE(15,fa);
        VARTRACE(15,fb); 
        VARTRACE(15,fc);
        VARTRACE(15,fs);

        SolverAction action = (*callback)(progress);

        if(action==Stop){
            break;
        }


    }
    
    system.updateSolution(b);
}


//////////////////////////////////////////////////////////////////////
