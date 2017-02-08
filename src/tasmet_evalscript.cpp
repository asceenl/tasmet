// tasmet_evalscript.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
// #define TRACERPLUS 16
#include "tasmet_evalscript.h"
#include "tasmet_exception.h"
#include "tasmet_tracer.h"
#include <sstream>
#include <muParser.h>

const int vector_prealloc = 30;

EvaluateFun::EvaluateFun(const string& fun_return,
                         const string& err_msg,
                         const string& var):
    _err_msg(err_msg),
    _fun_return(fun_return)
{

    TRACE(15,"EvaluateFun::EvaluateFun()");

    _parser = new mu::Parser();
    if(!_parser) throw TaSMETBadAlloc();

    /**
     * It is imperative to put a reserve operation here, which
     * preallocates enough space in the vector. Otherwise a
     * reallocation may invalidate the pointers which the parser
     * points to.
     * 
     */
    _values.reserve(vector_prealloc);

    _values.push_back(0);       // This is the (x), or (t)

    // Add pi, as it unfortunately does not exist
    _values.push_back(number_pi);
    try {
        _parser->DefineVar(var,&_values[0]);
        _parser->DefineVar("pi",&_values[1]);
        _parser->SetExpr(fun_return);
    }
    catch(mu::Parser::exception_type& e) {
        TRACE(15,e.GetMsg());
        throw TaSMETError(_err_msg);
    }
    
}
void EvaluateFun::addGlobalDef(const string& name,const d value) {
    TRACE(15,"EvaluateFun::addGlobalDef()");
    if(_values.size() == vector_prealloc -1)
        throw TaSMETError("Too many definitions");
    
    _values.push_back(value);
    try {    
        _parser->DefineVar(name,&_values[_values.size()-1]);
    }
    catch(mu::Parser::exception_type& e) {
        TRACE(15,e.GetMsg());
        throw TaSMETError(_err_msg);
    }
}
vd EvaluateFun::operator()(const vd& x) {
    TRACE(15,"EvaluateFun::operator(vd)");
    vd y(x.size());

    for(us i=0;i<x.size();i++) {
        _values[0] = x[i];
        VARTRACE(15,_values[0]);
        try {
            y(i) = _parser->Eval();
        }
        catch(mu::Parser::exception_type& e) {
            TRACE(15,e.GetMsg());
            throw TaSMETError(_err_msg);
        }
    }
    return y;
}

EvaluateFun::~EvaluateFun() {
    delete _parser;
}


//////////////////////////////////////////////////////////////////////
