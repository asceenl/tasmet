// solver_worker.h
//
// Author: J.A. de Jong 
//
// Description:
// Worker which solves the system, updates the progress graph,
// etc
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SOLVER_WORKER_H
#define SOLVER_WORKER_H
#include "tasmet_types.h"
#include "solver.h"
#include "system.h"
#include <atomic>
#include <QObject>
namespace pb{
    class System;
    class SolverParams;
}

class TaSystem;

Q_DECLARE_METATYPE(SolverProgress);

class SolverWorker: public QObject {
    Q_OBJECT

    std::atomic<bool> _run;
    Solver<GradientNonlinearSystem,vd>* _solver = nullptr;
    bool _converged = false;
    d _funtol,_reltol;
public:
    SolverWorker(const GradientNonlinearSystem& sys,
                 const pb::SolverParams& sparams);
    
    ~SolverWorker();

    vd getSolution() const { return _solver ? _solver->getSolution() : zeros<vd>(0);}

    /** 
     * Stop the solver. Called when the user presses the `stop' button
     */
    void solver_stop();
                      
public slots:
    void solver_start();
signals:
    
    // This signal is emitted when the solver is stopped
    void solver_stopped(bool converged);
    void progress(const SolverProgress&);
private:
    SolverAction pg_callback(SolverProgress pg);

};


#endif // SOLVER_WORKER_H
//////////////////////////////////////////////////////////////////////
