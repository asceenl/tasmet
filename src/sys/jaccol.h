// jaccol.h
//
// Author: J.A. de Jong 
//
// Description:
// A column block of the Jacobian matrix
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JACCOL_H
#define JACCOL_H
#include "tasmet_types.h"

class Variable;
class GlobalConf;


class JacCol{                // Column block of Jacobian
    bool tobeadded=true;
    us coldof_;                  // First dof of column
    dmat data_;                  // data
public:
    JacCol(const Variable&);
    JacCol(us coldof,const GlobalConf&);
    JacCol(us coldof,const dmat&);    
    JacCol(const Variable&,const dmat&);
    // JacCol(JacCol&&);           // Move data
    JacCol(const JacCol&);
    void prePostMultiply(const dmat& pre,const dmat& post){ data_=pre*data_*post;}
    JacCol& operator=(const JacCol&);
    // JacCol& operator=(JacCol&&);

    // Negate all terms
    JacCol operator-();
    bool isToAdd() const {return tobeadded;}
    void setToAdd(bool set){tobeadded=set;} // If this is set to
    // false, this column will not be added to the row.
    JacCol& operator+=(const dmat& add);
    JacCol& operator*=(const double&);
    dmat& data(){return data_;}
    const dmat& const_data() const {return data_;}
    const us& getColDof() const{return coldof_;}
    void show() const;
};
  

#endif // JACCOL_H
//////////////////////////////////////////////////////////////////////