// system_tools.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SYSTEM_TOOLS_H
#define SYSTEM_TOOLS_H
#include "system.pb.h"
#include "tasmet_tracer.h"
#include "tasmet_types.h"

// Load a system from a filepath
pb::System loadSystem(const string& filepath);

// Save a system to a filepath
void saveSystem(const string& filepath,const pb::System& sys);

// Returns true when the two systems are equal
bool compareSys(const pb::System& s1,const pb::System& s2);



#endif // SYSTEM_TOOLS_H
//////////////////////////////////////////////////////////////////////
