// tasmet_evalscript.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "tasmet_evalscript.h"
#include <chaiscript/chaiscript.hpp>
#include "chaiscript.h"
#include "tasmet_exception.h"
#include "tasmet_tracer.h"
#include <sstream>
using chaiscript::ChaiScript;
using std::stringstream;

template<typename T>
inline T wrap_eval(ChaiScript* chai,const string& script) {
   try {
       return chai->eval<T>(script);
    }
    catch(chaiscript::exception::eval_error &e) {
        TRACE(15,script);
        throw TaSMETError(e.what());
    }
   catch(std::bad_cast &e) {
        TRACE(15,script);
        throw TaSMETError("Cannot get return value from script");
    }

}
template<>
inline void wrap_eval<void>(ChaiScript* chai,const string& script) {
   try {
       chai->eval(script);
    }
   catch(std::runtime_error &e) {
        TRACE(15,script);
        throw TaSMETError(e.what());
    }
   catch(std::bad_cast &e) {
        TRACE(15,script);
        throw TaSMETError("Cannot get return value from script");
    }

}

EvaluateFun::EvaluateFun(const string& fun_return,
                         const string& err_msg):
    _err_msg(err_msg),
    _fun_return(fun_return)
{
    TRACE(15,"EvaluateFun::EvaluateFun()");
    _chai = getChaiScriptInstance();
    if(!_chai) throw TaSMETBadAlloc();

    string script = "def myfun(x) {\n";
    script += "return " + fun_return + "; }\n";
    
    wrap_eval<void>(_chai.get(),script);
    
}
void EvaluateFun::addGlobalDef(const string& name,const d value) {
    TRACE(15,"EvaluateFun::addGlobalDef()");

    _chai->add_global(chaiscript::var(value),name);
}
vd EvaluateFun::operator()(const vd& x) {
    TRACE(15,"EvaluateFun::operator(vd)");
    vd y(x.size());

    for(us i=0;i<x.size();i++) {

        stringstream value;
        value << "myfun(" << std::scientific << x(i) << ");";

        y(i) = wrap_eval<d>(_chai.get(),value.str());
        
    }
    return y;
}

EvaluateFun::~EvaluateFun() {

}


//////////////////////////////////////////////////////////////////////
