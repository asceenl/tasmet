// solver_thread.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "solver_worker.h"
#include <functional>
#include "tasmet_tracer.h"
#include "solver.h"
#include "system.pb.h"
#include "solver.pb.h"

#include <QThread>

// Solvers available
#include "newton_raphson.h"
#include "tasystem.h"

SolverWorker::SolverWorker(const GradientNonlinearSystem& sys,
                           const pb::SolverParams& sparams):
    _run(false),
    _funtol(sparams.funtol()),
    _reltol(sparams.reltol())
{
    TRACE(15,"SolverWorker");

    switch (sparams.solvertype()) {
    case pb::NewtonRaphson: {
        _solver = new NewtonRaphson(sys);
        break;
    }
    default:
        tasmet_assert(false,"Not implemented solver type");
        break;
    }
    
}
SolverWorker::~SolverWorker(){
    TRACE(15,"~SolverWorker");
    if(_solver!=nullptr) delete _solver;
}
void SolverWorker::solver_stop() {
    _run = false;
}

void SolverWorker::solver_start() {
    TRACE(15,"SolverWorker::solver_start()");
    using namespace std::placeholders; // for _1, _2 etc.    

    _run = true;

    progress_callback callback = std::bind(&SolverWorker::pg_callback,
                                           this,_1);

    
    tasmet_assert(_solver!=nullptr,"Solver not initialized");

    _solver->start(&callback);

    emit solver_stopped(_converged);
}
SolverAction SolverWorker::pg_callback(SolverProgress pg) {
    TRACE(15,"pg_callback");

    if(!_run) return Stop;

    emit progress(pg);

    if(pg.error) {
        _converged = false;
        return Stop;
    }
    if(pg.fun_err <= _funtol && pg.rel_err <= _reltol) {
        _converged = true;
        return Stop;
    }
    
    return Continue;


}



//////////////////////////////////////////////////////////////////////
