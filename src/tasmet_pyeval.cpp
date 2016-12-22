// tasmet_pyeval.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "tasmet_pyeval.h"
#include <PythonQt.h>
#include "tasmet_io.h"
#include "tasmet_exception.h"
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <sstream>

inline void getError(PythonQt* pyqt,string err_msg) {
    if(pyqt->hadError()) {
        pyqt->handleError();
        pyqt->clearError();

        throw TaSMETError(err_msg);
    }
}


EvaluateFun::EvaluateFun(const string& fun_return,
                         const string& err_msg):
    _err_msg(err_msg)
{

    _pyqt = PythonQt::self();

    try {
        getError(_pyqt,_err_msg);
    }
    catch(TaSMETError& e){
        cerr << "Uncleared error in script" << endl;
    }

    PythonQtObjectPtr context = _pyqt->getMainModule();

    std::stringstream script;

    script << "def myfun(x):\n    return ";
    script << fun_return << "\n";

    context.evalScript(QString::fromStdString(script.str()));

    // See if we have an error. If so, throw
    getError(_pyqt,_err_msg);

}
EvaluateFun::~EvaluateFun() {
    try {
        getError(_pyqt,_err_msg);
    }
    catch(...) {

    }
    PythonQtObjectPtr context = _pyqt->getMainModule();
    string delfun =  "del myfun(x):\n ";

    context.evalScript(QString::fromStdString(delfun));

}
void EvaluateFun::addGlobalDef(const string& name,const d value) {

    PythonQtObjectPtr context = _pyqt->getMainModule();

    std::stringstream script;
    script << name << " = " ;
    script << value << "\n";
        
    context.evalScript(QString::fromStdString(script.str()));        

    getError(_pyqt,_err_msg);

}
vd EvaluateFun::operator()(const vd& x) {

    PythonQtObjectPtr context = _pyqt->getMainModule();

    vd y(x.size());

    QVariant res;

    for(us i=0;i<x.size();i++) {

        QVariantList args;
        args << x(i);

        res = context.call("myfun",args);
        y(i) = res.toDouble();
            
        getError(_pyqt,_err_msg);
    }
    return y;
}


//////////////////////////////////////////////////////////////////////
