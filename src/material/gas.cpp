// gas.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "tasmet_tracer.h"
#include "tasmet_exception.h"
#include "gas.h"
#include "helium.h"
#include "air.h"
#include "nitrogen.h"

#include "tasmet_constants.h"

Gas* Gas::newGas(const GasType gastype,d T0,d p0) {

    if(T0>constants::maxT || T0< constants::minT)
        throw TaSMETError("Illegal reference temperature given");
    if(p0>constants::maxp || p0< constants::minp)
        throw TaSMETError("Illegal reference pressure given");
    // End sanity checks

    switch (gastype) {
    case helium:
        return new Helium(T0,p0);
        break;
    case air:
        return new Air(T0,p0);
    case nitrogen:
        return new Nitrogen(T0,p0);
    default:
        FATAL("Gas type not known");
        break;
    }
    return nullptr;
}


//////////////////////////////////////////////////////////////////////

