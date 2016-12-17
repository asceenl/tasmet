// duct.h
//
// Author: J.A. de Jong
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DUCT_H
#define DUCT_H
#include "segment.h"
#include "duct.pb.h"

class Equation;
class Drag;
class Heat;

class Duct {
    Duct(const string& name,const Geom& geom);
public:

    const Geom& geom() const;


    // Parsing a protobuf to generate a NEW Duct
    static Duct* parseProto(const pb::Duct&);


    // Solving

};

#endif // DUCT_H
//////////////////////////////////////////////////////////////////////
