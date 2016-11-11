// rottfuncs.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Wat doet dit bestand?
//////////////////////////////////////////////////////////////////////
#include "rottfuncs.h"
#include "bessel.h"

namespace {
    template <class array_type>
    array_type f_inviscid(const array_type& rh_over_delta){
        TRACE(0,"f_inviscid");
        return 0.0*rh_over_delta;
    }
    
    template <class array_type>
    array_type f_vert(const array_type& rh_over_delta){ //Vertical plate f_nu/f_k
        TRACE(0,"f_vert");
        c a=I+1.0;
        return tanh(a*rh_over_delta)/(a*rh_over_delta);
    }

    template <class array_type>
    array_type f_blapprox(const array_type& rh_over_delta){
        TRACE(0,"f_blapprox()");
        return (1.0-I)/rh_over_delta/2.0;
    }
  
    c f_circ(const c& rh_over_delta) {
        TRACE(0,"f_circ(c rh_ov_delta)");
        // vc s=sq2*rh_over_delta;
        c a(-1,1.0);
        c R_over_delta=2.0*rh_over_delta;
        c jvarg=a*R_over_delta;
        c j0=besselj0(jvarg);
        c j1_over_jxarg=besselj1_over_x(jvarg);;
        return 2.0*j1_over_jxarg/j0;
    }
    vc f_circ(const vc& rh_over_delta) {
        TRACE(0,"f_circ()");
        return vc(rh_over_delta).transform([] (c x) {return f_circ(x);});
    }

    c f_square(const c& rh_over_delta){
        TRACE(0,"f_square()");
        // Adjusted sligthly for improved speed.
        // Needs checking if rh is defined right.
        // For square pore with dimensions AxA : rh=S/PI = A^2 / 4A = A/4
        c F(0,0);
        c C(0,0);
        us n,m,msq,nsq;
        d pisq=pow(number_pi,2);
        for(n=1; n<11; n=n+2)
            {
                for(m = 1; m<11; m=m+2)
                    {
                        msq=m*m;
                        nsq=n*n;
                        C = 1.0-I*pisq/(32.0*pow(rh_over_delta,2))*((double)(msq+nsq));
                        F+=(1.0/msq/nsq)/C;
                    }
            }
        return 1.0-64.0/(pisq*pisq)*F;
    }
    // To transform this one would introduce an unallowable loop overhead
    vc f_square(const vc& rh_over_delta){
        TRACE(0,"f_square()");
        // Adjusted sligthly for improved speed.
        // Needs checking if rh is defined right.
        // For square pore with dimensions AxA : rh=S/PI = A^2 / 4A = A/4
        vc F(rh_over_delta.size(),fillwith::zeros);
        vc C(rh_over_delta.size(),fillwith::zeros);
        us n,m,msq,nsq;
        d pisq=pow(number_pi,2);
        for(n=1; n<11; n=n+2)
            {
                for(m = 1; m<11; m=m+2)
                    {
                        msq=m*m;
                        nsq=n*n;
                        C = 1.0-I*pisq/(32.0*pow(rh_over_delta,2))*(msq+nsq);
                        F+=(1.0/msq/nsq)/C;
                    }
            }
        return 1.0-64.0/(pisq*pisq)*F;
    }
}
RottFuncs::RottFuncs(const cshape shape):_shape(shape){
    const string shapestr = cshapeToString(shape);
    TRACE(8,"RottFuncs::RottFuncs(" << shapestr << ")");

    switch (shape) {
    case CIRC: 
        TRACE(10,"Fptr set to circ")
            f_ptr=&f_circ;        
        f_ptrc=&f_circ;
        break;
    case VERT:
        TRACE(10,"Fptr set to vert");
        f_ptr=&f_vert<vc>;
        f_ptrc=&f_vert<c>;
        break;
    case SQUARE:
        TRACE(10,"Fptr set to square");
        f_ptr=&f_square;
        f_ptrc=&f_square;
        break;
    case BLAPPROX:
        TRACE(10,"Fptr set to blapprox");
        f_ptr=&f_blapprox<vc>;
        f_ptrc=&f_blapprox<c>;
        break;
    case INVISCID:
        TRACE(10,"Fptr set to inviscid");
        f_ptr=&f_inviscid<vc>;
        f_ptrc=&f_inviscid<c>;
        break;
    default:
        FATAL("Unhandled shape");
        break;
    }
}

//////////////////////////////////////////////////////////////////////
