// duct.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "duct.h"
#include "tasystem.h"

Duct::Duct(const pb::Duct& duct):
    Segment(duct.name()),
    Geom(duct)
{

    


}
void Duct::updateSolution(const TaSystem&,const vd&) const {

}
vd Duct::getSolution(const TaSystem& sys,vd& sol,const us insert_start) {

    

}


Duct::Duct(const Duct& other):
    Segment(other),
    Geom(other)
{


}
us Duct::getNEqs(const TaSystem& sys) const {

    us Ns = sys.Ns();
    us number_eqs = _eqs.size();

    return Ns*number_eqs*ngp();
}

Duct* Duct::copy() const {
    return new Duct(*this);
}
//////////////////////////////////////////////////////////////////////
