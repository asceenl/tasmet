#include "triplets.h"
#include "tasmet_tracer.h"
#include "tasmet_io.h"
#include "tasmet_exception.h"

TripletList::operator sdmat() const {
      
    TRACE(15,"TripletList::operator sdmat()");
    
    vector<Triplet> validtrlist;

    validtrlist.reserve(triplets.size());

    for(auto& tr :triplets) {
        if(tr.valid) {
            validtrlist.push_back(tr);
        }
    }

    us nvals=validtrlist.size();

    arma::umat locations(2,nvals);
    
    vd values(nvals);

    for(us i=0;i<nvals;i++) {

        locations(0,i)=validtrlist[i].row;
        locations(1,i)=validtrlist[i].col;
        values(i)=validtrlist[i].value;
        TRACE(0,"row: " << validtrlist[i].row << ", col: "<< validtrlist[i].col << ", value: " << validtrlist[i].value);

    }
    
    TRACE(0,"ndofs: "<< _ndofs);

    return sdmat(true,locations,values,_ndofs,_ndofs);
}
void TripletList::addTriplet(const Triplet& t){

    if(t.row >= _ndofs || t.col >=_ndofs){
        throw TaSMETError("Invalid position for triplet");
    }
    triplets.push_back(t);

}
void TripletList::zeroOutRow(us rownr){
    TRACE(15,"zeroOutRow()");

    for(Triplet& tr: triplets){
        if(tr.row==rownr){
            tr.valid = false;
        }
    }
}


void TripletList::show()  const {
    for(const auto& t: triplets){
        cout << "Row: " << t.row << " , column: " << t.col << " , value: " << t.value << "\n";
    }
}
void TripletList::multiplyTriplets(const d& factor){
    TRACE(15,"multiplyTriplets()");
    for(auto tr: triplets){
        tr.value*= factor;
    }
}

void TripletList::reserveExtraDofs(us n){
    TRACE(15,"reserveExtraDofs()");
    us cursize=triplets.size();
    triplets.reserve(cursize+n);
}

void TripletList::shiftTriplets(int nrows,int ncols){
    // shift the position of the values in a matrix. nrows and ncols
    // can be negative numbers.
    TRACE(15,"shiftTriplets()");
    TRACE(100,"EXTRA CHECKS HERE!");
    for(auto tr: triplets){
        tr.col+=ncols;
        tr.row+=nrows;
    }
}



