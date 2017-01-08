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
#include <ostream>
#include "tasmet_types.h"
#include "tasmet_assert.h"
#include "system.h"

/**
 * Passed to the progress_callback function to keep track of the
 * Solver progress.
 * 
 */
struct SolverProgress
{
    size_t iteration = 0;       /**< The current iteration no. */
    d fun_err = 1e0;
    d rel_err = 1e0;
    bool error = false;          /**< Tells the callback that the
                                   Solver stops as an action of
                                   itself. Probably due to an internal
                                   error. */
};


/**
 * This enum should be return by the Solver callback function.
 * 
 */
enum SolverAction{
    Continue=0,                 /**< Tells the Solver to continue */
    Stop=1                      /**< Tells the Solver to stop. */
};

namespace pb{
    class SolverParams;
}

/// Typedef for the used callback which is called by a Solver instance.
typedef std::function<SolverAction(SolverProgress)> progress_callback;

/*! 
 * Returns a progress calback function that prints the progress
 * to the given ostream. This is a simple CLI callback function which
 * prints the solver progress to the given ostream as input. If no
 * ostream is given, the callback will print to stdout.
 *
 * @param funtol The required residual tolerance to stop the solver
 * @param reltol The required solution tolerance to stop the solver
 * @param out The ostream to output the progress to.
 *
 * @return A std::function object that can be passed to Solver::Start().
 */
progress_callback simple_progress_callback(d funtol,d reltol,
                                           std::ostream* out=nullptr);


template<typename system_T,typename result_T>
class Solver {

protected:
    system_T* _sys = nullptr;   /**< Stores and owns a system (creates
                                   a copy from the given System in the
                                   constructor */
public:
    Solver(const system_T& sys);
    Solver(const Solver&)=delete;
    Solver& operator=(const Solver&)=delete;
    
    /// Start the solver, using the given progress callback
    void start(progress_callback* callback);

    /// Returns the solution of the problem after the Solver is done
    /// solving.
    result_T getSolution();

    virtual ~Solver();

    /// Create a new Solver instance based on protobuf
    /// parameters. Throws a TaSMETError on error.
    static Solver* newSolver(const pb::SolverParams& params);


protected:
    /// This member fcn should be implemented by the Solver instance.
    virtual void start_implementation(system_T& sys,progress_callback*)=0;
};


#endif /* _SOLVER_H_ */

//////////////////////////////////////////////////////////////////////
