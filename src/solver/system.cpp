// system.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description: Instantiations for the SingleDofNoGradient class for
// the float and complex type
//////////////////////////////////////////////////////////////////////

#include "system.h"


template<typename T>
SingleDofNoGradient<T>::SingleDofNoGradient(const NoGradientNonlinearSystem<T>& wrapped_system) {

    this->wrapped_system = wrapped_system.copy();
}
template<typename T>
SingleDofNoGradient<T>::~SingleDofNoGradient(){
    delete this->wrapped_system;
}
template<typename T>
SingleDofNoGradient<T>* SingleDofNoGradient<T>::copy() const {
    return new SingleDofNoGradient<T>(*wrapped_system);
}

template<>
vd SingleDofNoGradient<d>::residual() const {
    d val = wrapped_system->residual();
    return vd({val});
}
template<>
vd SingleDofNoGradient<c>::residual() const {
    c val = wrapped_system->residual();
    return vd({val.real(),val.imag()});
}
template<>
vd SingleDofNoGradient<d>::getSolution() const {
    d val = wrapped_system->getSolution();
    return vd({val});
}
template<>
vd SingleDofNoGradient<c>::getSolution() const {
    c val = wrapped_system->getSolution();
    return vd({val.real(),val.imag()});
}

template<>
void SingleDofNoGradient<d>::updateSolution(const vd& new_guess){
    assert(new_guess.size()==1);
    wrapped_system->updateSolution(new_guess(0));
}
template<>
void SingleDofNoGradient<c>::updateSolution(const vd& new_guess){
    assert(new_guess.size()==2);
    wrapped_system->updateSolution(c(new_guess(0),new_guess(1)));
}

// Explicit intantiation for float-type and complex-type
template class SingleDofNoGradient<d>;
template class SingleDofNoGradient<c>;
//////////////////////////////////////////////////////////////////////
