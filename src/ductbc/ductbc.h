// ductbc.h
//
// Author: J.A. de Jong 
//
// Description:
// Prototype for duct boundary conditions
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DUCTBC_H
#define DUCTBC_H
#include "segment.h"

namespace pb{
    class DuctBc;
}

class DuctBc {

public:
    static Segment* newDuctBc(const us id,
                             const TaSystem& sys,
                             const pb::DuctBc&);
};


#endif // DUCTBC_H
//////////////////////////////////////////////////////////////////////
