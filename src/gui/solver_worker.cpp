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

#include "system.pb.h"
#include "solver.pb.h"

#include <QThread>


SolverWorker::SolverWorker(const pb::System& sys,const pb::SolverParams& sparams):
    _run(false),
    _reltol(sparams.reltol()),
    _funtol(sparams.funtol())
{

}
SolverWorker::~SolverWorker(){
    TRACE(15,"~SolverWorker");
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

    SolverProgress p;
    // For testing purposes

    SolverAction action;
    while(true) {
        TRACE(15,"Solver start virtual iteration");

        
        SolverAction action = callback(p);
        if(action != Continue) break;
        sleep(1);

        p.fun_err/=10;
        p.rel_err/=10;
        p.iteration++;

    }

    emit solver_stopped(_converged);
}
SolverAction SolverWorker::pg_callback(SolverProgress pg) {
    TRACE(15,"pg_callback");

    if(!_run) return Stop;

    emit progress(pg);

    if(pg.fun_err <= _funtol && pg.rel_err <= _reltol) {
        _converged = true;
        return Stop;
    }

    return Continue;


}



//////////////////////////////////////////////////////////////////////
