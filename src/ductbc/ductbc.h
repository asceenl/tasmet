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
#include "ductbc.pb.h"

class TaSystem;
class Duct;

class DuctBc :public Segment {
    pb::DuctBc _dbc;
public:
    DuctBc(const us id,
           const pb::DuctBc& dbc):
        Segment(id,dbc.name()),
        _dbc(dbc)    {}

    DuctBc(const DuctBc& o): Segment(o),_dbc(o._dbc) {}

    static DuctBc* newDuctBc(const us id,
                              const TaSystem& sys,
                              const pb::DuctBc&);

    const Duct& getDuct(const TaSystem&) const;

};


#endif // DUCTBC_H
//////////////////////////////////////////////////////////////////////
