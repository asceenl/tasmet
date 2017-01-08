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
#include "tasmet_io.h"

SolverAction ostream_progress_callback(SolverProgress pg,std::ostream* out, d funtol,d reltol) {

    // Create reference to ostream
    std::ostream& myout = (out) ? *out : std::cout;

    myout << "Iteration: " << pg.iteration;
    myout << " , Residual: " << pg.fun_err << " . Solution error: ";
    myout << pg.rel_err << endl;

    if(pg.fun_err <= funtol && pg.rel_err <= reltol) {
        myout << "Solution found within required tolerance" << endl;        
        return Stop;
    }
    
    return Continue;
}


progress_callback simple_progress_callback(d funtol,d reltol,std::ostream* out) {
    using namespace std::placeholders;  // for _1, _2, _3...
    using std::bind;
    return bind(ostream_progress_callback,_1,out,funtol,reltol);
    
}


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

        start_implementation(*_sys,callback);

}

template<typename system_T,typename result_T>
result_T Solver<system_T,result_T>::getSolution() {

    return _sys->getSolution();
}


// Explicit instantiation for some types of systems and results
template class Solver<NoGradientNonlinearSystem<vd>,vd>;
template class Solver<GradientNonlinearSystem,vd>;
template class Solver<NoGradientNonlinearSystem<d>,d>;
//////////////////////////////////////////////////////////////////////
