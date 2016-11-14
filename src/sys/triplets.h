#pragma once
#ifndef _TRIPLETS_H_
#define _TRIPLETS_H_

#include "tasmet_types.h"

struct Triplet{
    us row,col;
    d value;
    bool valid = true;

    Triplet(us row,us col,d value):row(row),col(col),value(value){}
};


class TripletList {

    // Size of the matrix to build eventually
    us _ndofs;

    vector<Triplet> triplets;
    
public:
    TripletList(us ndofs);
    ~TripletList(){}

    // Convert to Armadillo Sparse matrix
    operator sdmat() const;

    void setNdofs(us ndofs){_ndofs=ndofs;}

    void show() const;

    void addTriplet(const Triplet&);

    // Make one row zero
    void zeroOutRow(us rownr);

    TripletList& operator*=(d multiplicationfactor);

    // Add to capacity
    void reserveExtraDofs(us n);
    
    // Shift position of triplets a certain number of rows and cols.
    void shiftTriplets(int nrows,int ncols);

};



#endif /* _TRIPLETS_H_ */

