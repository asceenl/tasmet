// solver.h
//
// Author: J.A. de Jong 
//
// Description:
// When solve() is called, a copy of itself is created on the heap,
// for which a thread will be run. After the thread is done, the
// updated (solved) TaSystem will overwrite the old one.
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SOLVER_H
#define SOLVER_H
#include <functional>
#include <thread>
#include <atomic>

#include "tasmet_types.h"
#include "tasmet_assert.h"
#include "system.h"

struct SolverProgress
{
    size_t iteration = 0;
    d fun_err;
    d rel_err;
    bool done = false;
};

enum SolverAction{
    Continue=0,
    Stop = 1
};

typedef std::function<SolverAction(SolverProgress)> progress_callback;

template<typename system_T,typename result_T>
class Solver {

protected:
    system_T* _sys = nullptr;
    std::thread* _solver_thread = nullptr;
    std::atomic<bool> _running;
public:
    Solver(const system_T& sys);
    Solver(const Solver&)=delete;
    Solver& operator=(const Solver&)=delete;
    
    void start(progress_callback* callback=nullptr,bool wait=true);
    void stop();        // Stops the solver

    // Returns the solution of the problem
    result_T getSolution();

    virtual ~Solver();
    template<typename Y,typename rT>
    friend void SolverThread(Solver<Y,rT>*,Y*,progress_callback*);
protected:
    virtual void start_implementation(system_T& sys,progress_callback*)=0;
};


#endif /* _SOLVER_H_ */

//////////////////////////////////////////////////////////////////////