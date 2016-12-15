// tasmet_config.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_CONFIG_H
#define TASMET_CONFIG_H
#include <QString>
#include "tasmet_enum.h"

const QString appname = "TaSMET Ui";
const QString appversion = "1.0";
const QString company = "None";

// DECLARE_ENUM(SYSTEM_TYPE,DrivenSystem,EngineSystem)
DECLARE_ENUM(SystemType,Driven)

DECLARE_ENUM(SegmentType,Duct)

DECLARE_ENUM(SolverType,NewtonRaphson)




#endif // TASMET_CONFIG_H
//////////////////////////////////////////////////////////////////////

