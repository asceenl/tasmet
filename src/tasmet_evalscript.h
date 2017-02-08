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

namespace mu {
    class Parser;
}
// Helper class to evaluate simple 1D math function evaluations in Python
class EvaluateFun {
    string _err_msg;
    string _fun_return;
    mu::Parser* _parser = nullptr;
    std::vector<d> _values;
public:
    /** 
     * Construct a new function evaluator
     *
     * @param fun_return what the function should return
     * (e.g. sin(x)).  
     *
     * @param err_msg The exception message that
     * should be thrown in case something goes wrong.
     *
     * @param var The variable on which the function depends (e.g. x,
     * or t)
     */
    EvaluateFun(const string& fun_return,
                const string& err_msg = "Script error",
                const string& var = "x");
    
    /** 
     * Add a global parameter to the namespace.
     *
     * @param name 
     * @param value 
     */
    void addGlobalDef(const string& name,
                      const d value);
    
    /** 
     * Evaluate the  function at multiple points and return
     * the result for each point
     * 
     * @param points: the x-values, or t-values where to evaluate the
     * function.
     *
     * @return the results of the function evalated at the points
     */
    vd operator()(const vd& points);

    // Used to cleanup the python namespace
    ~EvaluateFun();
};

#endif // TASMET_EVALSCRIPT_H
//////////////////////////////////////////////////////////////////////
