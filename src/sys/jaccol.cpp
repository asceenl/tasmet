// jaccol.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "jaccol.h"
#include "tasmet_variable.h"


JacCol::JacCol(const Variable& thevar):
    coldof_(thevar.getDofNr()),
    data_(thevar.getGc()->Ns(),thevar.getGc()->Ns(),fillwith::zeros)
{  }
JacCol::JacCol(us coldof,const GlobalConf& gc):
    coldof_(coldof),
    data_(gc.Ns(),gc.Ns(),fillwith::zeros)
{  }
JacCol::JacCol(us coldof,const dmat& data):
    coldof_(coldof),
    data_(data)
{  }

JacCol::JacCol(const Variable& thevar,const dmat& data):
    coldof_(thevar.getDofNr()),
    data_(data)
{  }
// JacCol::JacCol(JacCol&& j):
//   tobeadded(std::move(j.tobeadded)),
//   coldof_(j.coldof_),
//   data_(std::move(j.data_))
// {
//   TRACE(45,"JacCol::JacCol(JacCol&&)");
// }
JacCol::JacCol(const JacCol& j):
    tobeadded(j.tobeadded),
    coldof_(j.coldof_),
    data_(j.data_)
{
    TRACE(5,"JacCol::JacCol(const JacCol& j)");
}
JacCol& JacCol::operator=(const JacCol& j){
    TRACE(10,"JacCol::operator=(const JacCol& j)");
    data_=j.data_;
    coldof_=j.coldof_;
    tobeadded=j.tobeadded;
    return *this;
}
// JacCol& JacCol::operator=(JacCol&& j){
//   TRACE(45,"JacCol::operator=(const JacCol& j)");
//   data_=std::move(j.data_);
//   coldof_=j.coldof_;
//   tobeadded=j.tobeadded;
//   return *this;
// }
  
JacCol& JacCol::operator+=(const dmat& data){
    data_+=data;
    return *this;
}
JacCol& JacCol::operator*=(const d& val){
    data_*=val;
    return *this;
}



//////////////////////////////////////////////////////////////////////
