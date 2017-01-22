#include "jacobian.h"
#include "triplets.h"
#include "tasmet_tracer.h"

void Jacobian::operator+=(const Jacobian& other){
    TRACE(2,"Jacobian::append()");
    jacrows.insert(jacrows.end(),other.jacrows.begin(),other.jacrows.end());
}
void Jacobian::operator+=(const JacRow& other){
    TRACE(2,"Jacobian::append()");
    jacrows.push_back(other);
}
// Jacobian::operator TripletList() const{

//     TRACE(18,"Jacobian::operator Tripletlist()");

//     int insertrow,insertcol;
//     TripletList res(ndofs_);
//     const dmat& typicaldatacel=jacrows.at(0).jaccols.at(0).data;
//     us size=typicaldatacel.n_rows;

//     us i,j;

//     for(const JacRow& row: jacrows) {
//         insertrow=row.getRowDof();
//         for(const JacCol& col: row.jaccols){
//             insertcol=col.getColDof();
//             if(insertcol>=0){
//                 const dmat& data=col.data;
//                 for(i=0;i<size;i++){
//                     for(j=0;j<size;j++){
//                         if(data(i,j)!=0)
//                             res.addTriplet(Triplet(i+insertrow,j+insertcol,data(i,j)));
//                     }
//                 }
//             } // insertcol>0
//         }   // for loop over cols
//     }     // for loop over rows
//     // TRACE(10,"SFSG");
//     return res;
// }
  

