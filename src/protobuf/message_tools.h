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
#include "tasmet_tracer.h"
#include "tasmet_types.h"

// Load a system from a filepath
template<typename T>
T loadMessage(const string& filepath);

// Save a system to a filepath
template<typename T>
void saveMessage(const string& filepath,const T& sys);

// Returns true when the two systems are equal
template<typename T>
bool compareSys(const T& s1,const T& s2);



#endif // SYSTEM_TOOLS_H
//////////////////////////////////////////////////////////////////////
