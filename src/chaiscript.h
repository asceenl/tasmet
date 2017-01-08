// chaiscript.h
//
// Author: J.A. de Jong 
//
// Description:
// Slow compiling chaiscript therefore we provide this wrapper
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CHAISCRIPT_H
#define CHAISCRIPT_H
namespace chaiscript {
    class ChaiScript;
}
#include <memory>

std::unique_ptr<chaiscript::ChaiScript> getChaiScriptInstance();

#endif // CHAISCRIPT_H
//////////////////////////////////////////////////////////////////////

