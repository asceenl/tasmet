// tasystem.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "segment.h"
#include "tasystem.h"
#include "triplets.h"
#include "jacobian.h"
#include "tasmet_assert.h"
#include "tasmet_exception.h"
#include "tasmet_constants.h"

TaSystem::TaSystem(const GlobalConf& gc,Gas::GasType gastype):
    _gc(new GlobalConf(gc)),
    _gas(Gas::newGas(gastype))
{
    TRACE(14,"TaSystem::TaSystem(gc,gastype)");
}
TaSystem::TaSystem(const TaSystem& o):
    _gc(o._gc),                 // Share a ptr to the Global conf
    _gas(Gas::newGas(Gas::GasType(*o._gas)))
{
    TRACE(25,"TaSystem::TaSystem(TaSystem&) copy");

    // Take over the pointers to the segments
    _segs = o._segs;

    for(auto& seg: _segs){
        seg.second = seg.second->copy();
    }

}
int TaSystem::getArbitrateMassEq(const vus& neqs) const {
    // Tells the TaSystem which Dof should be overwritten with the
    // mass arbitration equation. The special value of -1 means, that
    // mass is not arbitrated. This can be the case if for example a
    // pressure boundary condition is present.
    int arbitrateMassEq=-1;
    bool masseq_found = false;

    us total_neqs = 0, i=0;

    // Iterate over all segments to find a single one segment Dof that is
    // willing to arbitrate the mass in the system. This can, of
    // course, be only be one segment.
    int local_mass_eq = -1;
    
    const Segment* seg;
    for(auto& seg_: _segs) {
        seg = seg_.second;

        if(masseq_found && (local_mass_eq = seg->arbitrateMassEq())) {
            throw TaSMETError("Error: only one Segment is allowed to"
                              " arbitrate the system mass");
        }
        else if(local_mass_eq != -1){

            arbitrateMassEq = total_neqs+local_mass_eq;

            masseq_found = true;
        }

        total_neqs += neqs(i);
        i++;
    }
    return arbitrateMassEq;
}
TaSystem& TaSystem::operator+=(const Segment& s){

    TRACE(24,"TaSystem::operator+=(Seg)");

    if(_segs.find(s.getid())!=_segs.end()){
        std::stringstream error;
        error << "Segment with id " << s.getid() <<
            "already present in the system";
        throw TaSMETError(error);
    }
    _segs[s.getid()]=s.copy();
    return *this;
}
void TaSystem::updateSolution(const vd& sol) {
    TRACE(15,"TaSystem::updateSolution()");

    us firstdof = 0;
    us ndofs;
    Segment* seg;
    for(auto& seg_: _segs) {
        seg = seg_.second;
        ndofs = seg->getNDofs(*this);

        firstdof += ndofs;

        seg->updateSolution(*this,sol.subvec(firstdof,firstdof+ndofs-1));
    }
}
vd TaSystem::getSolution() const {
    us firstdof = 0,i=0;
    vus ndofs = getNDofs();
    us total_ndofs = arma::sum(ndofs);
    vd sol(total_ndofs);

    #ifdef TASMET_DEBUG
    sol.zeros();
    #endif

    Segment* seg;
    for(auto& seg_: _segs) {
        seg = seg_.second;
        seg->getSolution(*this,sol,firstdof);
        firstdof += ndofs(i);
        i++;
    }
    return sol;
}
vd TaSystem::residual() const {
    TRACE(15,"TaSystem::residual()");

    vus neqs = getNEqs();
    us total_neqs = arma::sum(neqs);
    vus eqstart(neqs.size());

    us i=0;

    for(us& eqs : neqs) {

        if(i>0) {
            eqstart(i) = eqstart(i-1) + eqs;
        }
        else {
            eqstart(i) = 0;
        }
        i++;                 
    }
    assert(i==_segs.size()-1);

    if(total_neqs>constants::maxndofs)      {
        throw TaSMETError("Too many DOFS required."
                          " Problem too large.");
    }

    int arbitrateMassEq = getArbitrateMassEq(neqs);

    // This is the mass in the sytem. Only counted when there is an
    // equation present which arbitrates the mass in the system
    d mass=0;
    
    VARTRACE(25,total_neqs);
    VARTRACE(25,eqstart);

    vd residual(total_neqs);

    i=0;
    const Segment* seg;
    for(auto seg_: _segs) {

        seg = seg_.second;

        // Put the residual of the segment in the over-all residual
        seg->residual(*this,residual,eqstart(i));

        // Count the mass, add it
        if(arbitrateMassEq!=-1) {
            mass += seg->getMass(*this);
        }
        
        i++;
    }

    #ifdef TASMET_DEBUG
    assert(arbitrateMassEq< (int) total_neqs);    
    #endif // TASMET_DEBUG

    // Exchange equation if we need to arbitrate mass
    if(arbitrateMassEq!=-1) {
        residual(arbitrateMassEq)=mass - _mass;
    }

    return residual;
}

vus TaSystem::getNDofs() const  {
    TRACE(0,"TaSystem::getNDofs()");
    vus Ndofs(_segs.size());
    us i=0;
    for (auto seg : _segs) {
        Ndofs(i)=seg.second->getNDofs(*this);
        i++;
    }
    return Ndofs;
}
vus TaSystem::getNEqs() const  {
    TRACE(0,"TaSystem::getNDofs()");
    vus Neqs(_segs.size());
    us i=0;
    for (auto seg :_segs) {
        Neqs(i)=seg.second->getNEqs(*this);
        i++;
    }
    return Neqs;
}
void TaSystem::show(us detailnr){
    TRACE(25,"TaSystem::show()");

    cout << "########################## Showing TaSystem...\n";
    cout << "Showing Global configuration...\n";
    _gc->show();
    
    if(detailnr>0){
        for(auto seg:_segs){
            cout << "Showing segment with ID " << seg.first << "\n";
            seg.second->show(*this,detailnr);
        }
    } // detailnr>0
}

TripletList TaSystem::jacTriplets() const {

    TRACE(14,"TaSystem::jacobian()");
    vus ndofs=getNDofs();
    vus neqs=getNEqs();
    us total_ndofs = arma::sum(ndofs);
    us total_neqs = arma::sum(neqs);
    us i=0,dofnr=0,eqnr=0;
    Jacobian j(total_ndofs);

    int arbitrateMassEq = getArbitrateMassEq(neqs);
    vd dmtotdx;
    if(arbitrateMassEq > -1)
        dmtotdx = vd(total_ndofs);

    const Segment* seg;
    for(auto& seg_ :_segs) {
        seg = seg_.second;
        seg->jac(*this,j,dofnr,eqnr);
        if(arbitrateMassEq > -1) {
            seg->dmtotdx(*this,dmtotdx,dofnr);
        }
        eqnr += neqs(i);
        dofnr += ndofs(i);
        i++;
    }

    if(dofnr!=eqnr){
        throw TaSMETError("System of equations is over/underdetermined");
    }
    
    // Convert to tripletlist
    TripletList jac=j;

    assert(arbitrateMassEq< (int) total_neqs);

    // Exchange equation if we need to arbitrate mass
    if(arbitrateMassEq!=-1) {
        // Replace this equation with global mass conservation
        jac.zeroOutRow(arbitrateMassEq);

        us dmtotdxsize=dmtotdx.size();
        for(us k=0;k<dmtotdxsize;k++)
            if(dmtotdx(k)!=0){
                // TRACE(20,"k: " << k);
                // TRACE(20,"dmtotdx:"<< dmtotdx(k));
                jac.addTriplet(Triplet(arbitrateMassEq,k,dmtotdx(k)));
            }
    }

    return jac;
}
sdmat TaSystem::jacobian() const {
    TRACE(14,"TaSystem::Jac()");
    return jacTriplets();            // Implicitly converts to sdmat
}
void TaSystem::resetHarmonics(){
    for(auto seg: _segs) {
        seg.second->resetHarmonics();
    }
}
dmat TaSystem::showJac(){

    TRACE(15,"TaSystem::showJac()");

    return dmat(jacobian());
}

TaSystem::~TaSystem() {
    TRACE(25,"~TaSystem()");

    for(auto& seg: _segs){
        delete seg.second;
    }
    delete _gas;
}


//////////////////////////////////////////////////////////////////////
