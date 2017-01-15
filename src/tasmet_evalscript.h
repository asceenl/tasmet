// tasmet_evalscript.h
//
// Author: J.A. de Jong 
//
// Description:
//
// Description: This file implements a helper class to evaluate simple
// 1D single-parameter python math functions
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_EVALSCRIPT_H
#define TASMET_EVALSCRIPT_H
#include "tasmet_types.h"
#include <memory>

namespace chaiscript {
    class ChaiScript;
}

// Helper class to evaluate simple 1D math function evaluations in Python
class EvaluateFun {
    std::unique_ptr<chaiscript::ChaiScript> _chai;
    string _err_msg;
    string _fun_return;
public:
    EvaluateFun(const string& fun_return,const string& err_msg = "Script error",const string& vars = "x");
    
    // Add a global definition to the namespace
    void addGlobalDef(const string& name,
                      const d value);
    
    // Evaluate a single function at multiple points and return
    // the result for each point
    vd operator()(const vd& points);

    // Used to cleanup the python namespace
    ~EvaluateFun();
};

#endif // TASMET_EVALSCRIPT_H
//////////////////////////////////////////////////////////////////////
