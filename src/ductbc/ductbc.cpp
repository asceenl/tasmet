// ductbc.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "ductbc.h"
#include "ductbc.pb.h"
#include "tasmet_tracer.h"
#include "tasmet_assert.h"
#include "pressurebc.h"
#include "adiabaticwall.h"
#include "tasystem.h"
#include "duct.h"

const Duct& DuctBc::getDuct() const {

    return dynamic_cast<const Duct&>(sys.getSegment(_dbc.duct_id()));

}

DuctBc* DuctBc::newDuctBc(const us id,
                          const TaSystem& sys,
                          const pb::DuctBc& dbc) {

    TRACE(15,"newDuctBc");

    switch (dbc.type()) {
    case pb::PressureBc: {
        return new PressureBc(sys,id,dbc);
        break;
    }
    case pb::AdiabaticWall: {
        return new AdiabaticWall(sys,id,dbc);
        break;
    }
    default:
        tasmet_assert(false,"Not implemented DuctBc");
        break;
    }
    return nullptr;

}

//////////////////////////////////////////////////////////////////////
