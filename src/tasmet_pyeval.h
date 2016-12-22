// tasmet_pyeval.h
//
// Author: J.A. de Jong 
//
// Description: This file implements a helper class to evaluate simple
// 1D single-parameter python math functions
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_PYEVAL_H
#define TASMET_PYEVAL_H
#include "tasmet_types.h"

class PythonQt;

// Helper class to evaluate simple 1D math function evaluations in Python
class EvaluateFun {

    PythonQt* _pyqt;
    string _err_msg;
public:
    EvaluateFun(const string& fun_return,const string& err_msg = "Script error");
    
    // Add a global definition to the namespace
    void addGlobalDef(const string& name,
                      const d value);
    
    // Evaluate a single function at multiple points and return
    // the result for each point
    vd operator()(const vd& points);

    // Used to cleanup the python namespace
    ~EvaluateFun();
};



#endif // TASMET_PYEVAL_H
//////////////////////////////////////////////////////////////////////
