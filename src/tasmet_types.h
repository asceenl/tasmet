// tasmet_types.h
//
// Author: J.A. de Jong 
//
// Description:
// Typedefs and namespace pollution for stuff that is almost always
// needed.
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_TYPES_H
#define TASMET_TYPES_H
#include <type_traits>

// We require C++
#ifndef __cplusplus
#error The c++ compiler should be used.
#endif

// Some header files I (almost) always need
#include <vector>		// C++ std vector
#include <string>		// C++ string class
#include <complex>
#include <armadillo>		// Our linear algebra package

typedef size_t us;		  /* Size type I always use */

#define fillwith arma::fill	    // Fill vector or matrix with ...

// To change the whole code to 32-bit floating points, change this to
// float.
#if TASMET_FLOAT == 32
typedef float d;		/* Shortcut for double */			
#elif TASMET_FLOAT == 64
typedef double d;		/* Shortcut for double */			
#else
static_assert(false,"TASMET_FLOAT should be either 32 or 64");
#endif

typedef std::complex<d> c; /* Shortcut for c++ complex number */

using arma::abs;
using std::string;		/* C++ Strings */				
using std::vector;              // C++ Vectors

/* Armadillo functions and objects we often need */				
using arma::sin;
using arma::cos;
using arma::exp;
using arma::ones;								
using arma::zeros;								
using arma::eye;								
using arma::inv;								
using arma::det;								
using arma::log_det;								
using arma::norm;								
using arma::span;								
using arma::linspace;								
using arma::diagmat;								

// Constants
// I need these numbers so often, that they can be in the global namespace

const c I(0,1); //Complex unity
const c sqI=sqrt(I);
const d sq2=sqrt(2.0);
const c sqmI=sqrt(-1.0*I);
const c minI(0,-1); //Minus complex unity
const d number_pi=arma::datum::pi; // The number 3.14159265359..

template<size_t i>
using vdi = arma::Col<d>::fixed<i>;
template<size_t i>
using vci = arma::Col<c>::fixed<i>;

typedef arma::Col<d> vd;		  /* Column vector of doubles */		
typedef arma::Col<c> vc;	  /* Column vector of complex numbers */	
typedef arma::Col<us> vus;	  /* Column vector of unsigned integers */	
typedef arma::Mat<d> dmat; /* (Dense) Matrix of doubles */		
typedef arma::Mat<c> cmat;	  /* (Dense) matrix of complex numbers */	

typedef arma::SpMat<d> sdmat;	  /* Sparse matrix of doubles */		


#endif // TASMET_TYPES_H
//////////////////////////////////////////////////////////////////////

