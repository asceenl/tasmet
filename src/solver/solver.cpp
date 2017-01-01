// solver.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "tasmet_tracer.h"
#include "solver.h"
#include "tasmet_exception.h"
#include <chrono>


template<typename system_T,typename result_T>
static void SolverThread(Solver<system_T,result_T>* solver,
                         system_T* system,
                         progress_callback* callback);


template<typename system_T,typename result_T>
Solver<system_T,result_T>::Solver(const system_T& sys){
    _sys = sys.copy();
    if(!_sys) throw TaSMETBadAlloc();
}

template<typename system_T,typename result_T>
Solver<system_T,result_T>::~Solver(){

    delete _sys;
    
}

template<typename system_T,typename result_T>
void Solver<system_T,result_T>::start(progress_callback* callback){

        TRACE(15,"Waiting for solver...");
        start_implementation(*_sys,callback);

}


template<typename system_T,typename result_T>
result_T Solver<system_T,result_T>::getSolution() {

    return _sys->getSolution();
}
template<typename system_T,typename result_T>
void SolverThread(Solver<system_T,result_T>* solver,system_T* system,progress_callback* callback) {
    assert(system);
    
    solver->_running = true;

    solver->start_implementation(*system,callback);

    solver->_running = false;

}


// Explicit instantiation for some types of systems and results
template class Solver<NoGradientNonlinearSystem<vd>,vd>;
template class Solver<GradientNonlinearSystem,vd>;
template class Solver<NoGradientNonlinearSystem<d>,d>;
//////////////////////////////////////////////////////////////////////
