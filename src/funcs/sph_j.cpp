// sph_j.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include <gsl/gsl_sf_bessel.h>
#include "sph_j.h"
#include "tasmet_exception.h"


vd sph_j(int n,d x){
    if (n < 1)
        throw TaSMETError("n needs to be > 1");
    vd result(n);
    gsl_sf_bessel_jl_steed_array(n,x,result.memptr());
    return result;
}

dmat sph_j(int n, const vd& x){
    if (n < 1)
        throw TaSMETError("n needs to be > 1");

    dmat result(x.size(),n);
    for (us i = 0; i < x.size(); i++)
        result.row(i) = sph_j(n, x(i)).t();
    return result;
}


//////////////////////////////////////////////////////////////////////
