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
    DuctBc(const TaSystem& sys,
           const us id,
           const pb::DuctBc& dbc):
        Segment(sys,id,dbc.name()),
        _dbc(dbc)    {}

    DuctBc(const TaSystem&sys,const DuctBc& o):
        Segment(sys,o),
        _dbc(o._dbc) {}

    static DuctBc* newDuctBc(const us id,
                             const TaSystem& sys,
                             const pb::DuctBc&);

    const Duct& getDuct() const;

    virtual void initialSolution(SegPositionMapper&) const {}

};


#endif // DUCTBC_H
//////////////////////////////////////////////////////////////////////
