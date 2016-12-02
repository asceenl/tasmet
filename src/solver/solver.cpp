// solver.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

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
    if(!_sys)
        throw TasMETBadAlloc();
    _running = false;
}

template<typename system_T,typename result_T>
Solver<system_T,result_T>::~Solver(){

    stop();
    assert(!_running);
    assert(!_solver_thread);
    delete _sys;
    
}

template<typename system_T,typename result_T>
void Solver<system_T,result_T>::start(progress_callback* callback,bool wait){

    if(_running){
        assert(_solver_thread);
        throw TaSMETError("Solver already running");
    }

    assert(_solver_thread == nullptr);

    this->_solver_thread = new std::thread(SolverThread<system_T,result_T>,
                                           this,
                                           _sys,
                                           callback);
    if(!_solver_thread)
        throw TasMETBadAlloc();

    if(wait){
        while (_running){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        // Cleanup resources
        stop();
    }
}

template<typename system_T,typename result_T>
void Solver<system_T,result_T>::stop() {

    _running = false;
    if(_solver_thread){

        _solver_thread->join();
        delete _solver_thread;
        _solver_thread = nullptr;

    }
}
template<typename system_T,typename result_T>
result_T Solver<system_T,result_T>::getSolution() const{
    if(_running){
        throw TaSMETError("Solver is running");
    }
    // Cleanup thread resources
    stop();
    return _sys->getSolution();
}
template<typename system_T,typename result_T>
void SolverThread(Solver<system_T,result_T>* solver,system_T* system,progress_callback* callback) {
    assert(system);
    
    solver->_running = true;

    solver->start_implementation(*system,callback);

    solver->_running = false;

}


// Explicit instantiation for a
template class Solver<NoGradientNonlinearSystem<vd>,vd>;
template class Solver<GradientNonlinearSystem,vd>;
template class Solver<NoGradientNonlinearSystem<d>,d>;
//////////////////////////////////////////////////////////////////////
