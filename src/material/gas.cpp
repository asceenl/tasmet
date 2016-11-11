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

Gas* Gas::newGas(const GasType gastype) {

    switch (gastype) {
    case helium:
        return new Helium();
        break;
    case air:
        return new Air();
    case nitrogen:
        return new Nitrogen();
    default:
        FATAL("Gas type not known");
        break;
    }
    return nullptr;
}


//////////////////////////////////////////////////////////////////////

