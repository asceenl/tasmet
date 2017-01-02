// tasmet_solvemodel.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Program which can be used to solve a TaSMET Model from the CLI
//////////////////////////////////////////////////////////////////////
#include "protobuf/system_tools.h"       // loadSystem, saveSystem
#include "tasmet_io.h"
#include "tasmet_exception.h"
#include "sys/tasystem.h"

// For using python from within Qt
#include <PythonQt.h>

void usage(const char* fn) {
    cout << "Usage: " << endl;
    cout << fn << " [Input file] " << endl;
}

int main(int argc, char *argv[]) {

    INITTRACE(10);

    // Initialize PythonQt
    // PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    PythonQt::init();
    
    PythonQt* pyqt = PythonQt::self();
    PythonQtObjectPtr context = pyqt->getMainModule();

    QVariant rv = context.evalScript("from math import *\n");
    if(pyqt->hadError()) {
        return -1;
    }

    if(argc != 2) {
        usage(argv[0]);
        return -1;
    }

    pb::System sys;
    
    string filename = argv[1];
    try {
        sys = loadSystem(filename);
    }
    catch(TaSMETError &e) {
        cerr << e.what() << endl;
        return -1;
    }

    cout << "Loaded model: " << endl;
    cout << sys.DebugString();

    TaSystem* system;

    try {
        system = new TaSystem(sys);
    }
    catch(TaSMETError &e) {
        cerr << "Model initialization error: " << endl;
        cerr << e.what() << endl;
        return -1;
    }

}


//////////////////////////////////////////////////////////////////////
