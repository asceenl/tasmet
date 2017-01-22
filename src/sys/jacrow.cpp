#include "jacobian.h"
#include "tasmet_variable.h"
#include "globalconf.h"


JacRow& JacRow::operator+=(const JacCol& j){
    TRACE(10,"JacRow::operator+=(const JacCol& j)");
    jaccols.push_back(j);
    return *this;
}

JacRow& JacRow::operator*=(const d val){
    TRACE(2,"Jacobian::operator*=()");
    for(JacCol& col: jaccols)
        col*=val;
    return *this;
}
JacRow& JacRow::operator+=(const JacRow& other){
    // Reserve some space
    this->jaccols.reserve(this->jaccols.capacity()+other.jaccols.size()-this->jaccols.size());

    for(const JacCol& col :other.jaccols)
        (*this)+=col;

    return *this;
}
JacRow JacRow::operator-() const{
    TRACE(15,"JacRow::operator-()");

    JacRow result(*this);
    for (JacCol& jaccol : result.jaccols) 
        jaccol*=-1;

    return result;
}
void JacRow::prePostMultiply(const dmat& pre,const dmat& post) {
    for(JacCol& jaccol: jaccols)
        jaccol.prePostMultiply(pre,post);
}

