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
#include <QThread>
#include <qcustomplot.h>
SolverWorker::SolverWorker(pb::System& sys):
    _run(false)
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
    while(true) {
        TRACE(15,"Solver start virtual iteration");

        
        SolverAction action = callback(p);
        if(action == Stop) break;
        sleep(1);

        p.fun_err/=10;
        p.rel_err/=10;
        p.iteration++;
    }

    emit solver_stopped();
}
SolverAction SolverWorker::pg_callback(SolverProgress pg) {
    TRACE(15,"pg_callback");

    if(!_run) return Stop;


    // QCPGraph *_funer,*_reler,*_funtol,*_reltol;

    // _funer = _plot->addGraph();
    // _reler = _plot->addGraph();

    TRACE(15,"SolverWorker::pg_callback()");

    // VARTRACE(15,progress.fun_err);
    // VARTRACE(15,progress.iteration);
    // _funer->addData(progress.iteration,progress.fun_err);
    // _reler->addData(progress.iteration,progress.rel_err);    

    // _plot->xAxis->setRange(0,progress.iteration);

    // _plot->replot();

    // if(progress.done) {
    //     // We are done!

    //     QString m = "Solver reached a converged solution.";

    //     QMessageBox::information(this,
    //                              "Solver done",
    //                              m);

        
    // }


    return Continue;


}



//////////////////////////////////////////////////////////////////////
