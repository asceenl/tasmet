// jacrow.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JACROW_H
#define JACROW_H
#include <vector>
#include "jaccol.h"

class JacRow{                 // Row in Jacobian matrix

public:

    PosId id;                   /**< Specifies the row for this
                                   JacRow */
    std::vector<JacCol> jaccols; /**< The columns in this row */

    /** 
     * Initialize a Jacobian row with a single column block
     *
     * @param id The equation ID
     * @param col The added column
     */
    JacRow(PosId id,const JacCol& col):JacRow(id){jaccols.push_back(col);}

    JacRow(PosId id): id(id){}

    JacRow operator-() const;

    JacRow& operator+=(const JacCol&);
    JacRow& operator+=(const JacRow& jacrow);
    JacRow& operator*=(const d val); // Multiply all terms with constant value

    /** 
     * Pre- and postmultiply all Column blocks with the given
     * matrices. Hence the new columns will be col_new = pre*col_old*post
     *
     * @param pre : The pre-multiply matrix
     * @param post: The post-multiply matrix
     */
    void prePostMultiply(const dmat& pre,const dmat& post);


};

  



#endif // JACROW_H
//////////////////////////////////////////////////////////////////////
