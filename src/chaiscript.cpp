// chaiscript.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "chaiscript.h"
#include <chaiscript/chaiscript_stdlib.hpp>
#include <chaiscript/chaiscript.hpp>
#include "chaiscript/math.hpp"
#include "tasmet_types.h"

using chaiscript::ChaiScript;

std::unique_ptr<chaiscript::ChaiScript> getChaiScriptInstance() {

    auto mathlib = chaiscript::extras::math::bootstrap();
    
    std::unique_ptr<ChaiScript> chai(new ChaiScript());
    // std::unique_ptr<ChaiScript> chai(new ChaiScript(chaiscript::Std_Lib::library()));
    
    chai->add(mathlib);
    chai->add_global(chaiscript::var(number_pi),"pi");
    return chai;
}
//////////////////////////////////////////////////////////////////////
