// tasmet_solvemodel.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Program which can be used to solve a TaSMET Model from the CLI
//////////////////////////////////////////////////////////////////////
#include "protobuf/message_tools.h"       // loadSystem, saveSystem
#include "tasmet_io.h"
#include "tasmet_exception.h"
#include "sys/tasystem.h"
#include "solver/newton_raphson.h"
#include "protobuf/model.pb.h"
#include <memory>


void usage(const char* fn) {
    cout << "Usage: " << endl;
    cout << fn << " [Input file] ";
    #ifdef TASMET_DEBUG
    cout << "[Tracer_Level]";
    #endif
    cout << endl;
}

int main(int argc, char *argv[]) {



    if(argc < 2 || argc > 3) {
        usage(argv[0]);
        return -1;
    }
    #ifdef TASMET_DEBUG
    int trace_level = 20;

    if(argc >= 3) {
        try {
            trace_level = std::stoi(argv[2]);
        }
        catch(...) {
            usage(argv[0]);
            return -1;
        }
    }
    INITTRACE(trace_level);
    #endif

    pb::Model model;
    
    string filename = argv[1];
    try {
        model = loadMessage<pb::Model>(filename);
    }
    catch(TaSMETError &e) {
        cerr << e.what() << endl;
        return -1;
    }

    cout << "Loaded model: " << endl;
    cout << model.system().DebugString();

    std::unique_ptr<TaSystem> system;

    try {
        system = std::unique_ptr<TaSystem>(new TaSystem(model.system()));
    }
    catch(TaSMETError &e) {
        cerr << "Model initialization error: " << endl;
        cerr << e.what() << endl;
        return -1;
    }

    NewtonRaphson solver(*system.get());

    progress_callback cb = simple_progress_callback(1e-6,1e-6);

    solver.start(&cb);


    vd sol = solver.getSolution();

    // Put solution in our copy of system
    system->updateSolution(sol);

    system->exportHDF5(filename+".h5");

}


//////////////////////////////////////////////////////////////////////
