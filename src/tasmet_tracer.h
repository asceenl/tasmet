// tasmet_tracer.h
//
// Author: J.A. de Jong 
// Description:
// When this file is included, we are able to use the following macro's:
// TRACE(10, "Test");
// 
// double x=2;
// VARTRACE(2,x);
//
// WARN("Warning message");
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_TRACER_H
#define TASMET_TRACER_H
#include <type_traits>

// **************************************** Tracer code
// If PP variable TRACER is not defined, we automatically set it on.
#ifndef TRACER
static_assert(false,"TRACER macro not defined");
#endif

// Not so interesting part
#define rawstr(x) #x
#define namestr(x) rawstr(x)
#define annestr(x) namestr(x)
#define FILEWITHOUTPATH ( strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__ )
#define POS FILEWITHOUTPATH << ":" << __LINE__ <<  ": "
// End not so interesting part

#define RAWWARNING(a) std::cout << red << a << def << "\n";
#define WARN(a) RAWWARNING(POS << "WARNING: " << a)

// SHOULD NOT BE USED IN A LIBRARY!!
#define FATAL(a) WARN(a); abort(); 

#if TRACER == 1
#include "tasmet_consolecolors.h"
#include <iostream>

// This variable can be used to increase the trace level of one
// specific file at compile time
#ifndef TRACERPLUS
#define TRACERPLUS (0)
#endif


// Initialize MAXTRACELEVEL and BUILDINTRACELEVEL
#ifndef MAXTRACELEVEL
#define MAXTRACELEVEL (5000) 	// To which the TRACELEVEL initially is set
#endif

// Define this preprocessor definition to overwrite
// Use -O flag for compiler to remove the dead functions!
// In that case all cout's for TRACE() are removed from code
#ifndef BUILDINTRACELEVEL
#define BUILDINTRACELEVEL (-10)
#endif

extern int tasmet_tracer_level;
// Use this preprocessor command to introduce one tasmet_tracer_level integer per unit
/* Introduce one static logger */
// We trust that the compiler will eliminate 'dead code', which means
// that if variable BUILDINTRACERLEVEL is set, the inner if statement
// will not be reached.
#define WRITETRACE(level,a) \
    if(level>=BUILDINTRACELEVEL && level >=tasmet_tracer_level) {       \
        std::cout << a << "\n";                                         \
    }


#define TRACE(l,a) WRITETRACE(l+TRACERPLUS,annestr(tasmet_tracer_level) << "-" << (l)<< "-" << POS << a)
#define VARTRACE(l,a) TRACE(l,#a " = " << a)

#define INITTRACE(ll)                                                   \
    std::cout << "INITTRACE with tracelevel " << ll << " for " << annestr(tasmet_tracer_level) << "\n"; \
    tasmet_tracer_level=ll;
#else  // TRACER !=1

#define VARTRACE(l,a) 
#define TRACE(l,a)
#define INITTRACE(a)
#define WARN(a)


#endif	// ######################################## TRACER ==1
#endif // TASMET_TRACER_H
//////////////////////////////////////////////////////////////////////
