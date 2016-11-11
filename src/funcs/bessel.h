// bessel.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BESSEL_H
#define BESSEL_H
#include "tasmet_types.h"

// Bessel function of first kind and zero'th order for complex numbers
// j_0(x)
c besselj0(c x);

// Bessel function of first kind and first order for complex numbers:
// j1(x)/x
c besselj1_over_x(c x);


#endif // BESSEL_H
//////////////////////////////////////////////////////////////////////
