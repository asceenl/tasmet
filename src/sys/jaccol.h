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

/**
 * A PosId is a specifier for the position in the global Jacobian
 * matrix. Both considering the column as well as the row
 * 
 */
struct PosId {
    int segno;
    int segoffset;
};

class JacCol{                // Column block of Jacobian

public:

    // For more ease of use, we make the members public.

    PosId id;                  /**< The Id of this DOF */
    dmat data;                 /**< The matrix block */

    JacCol(PosId id): id(id){}
    JacCol(PosId id,const dmat& data):id(id),data(data){}

    JacCol& operator=(const JacCol& o) { id = o.id; data=o.data; return *this; }
    JacCol(const JacCol& o): id(o.id),data(o.data){}

    void prePostMultiply(const dmat& pre,const dmat& post){ data=pre*data*post;}

    // Some operations on the data
    JacCol operator-() { return JacCol(id,-data);}

    JacCol& operator+=(const dmat& add) {data+=add; return *this;}
    JacCol& operator*=(const double d) {data*=d; return *this;}

};
  

#endif // JACCOL_H
//////////////////////////////////////////////////////////////////////
