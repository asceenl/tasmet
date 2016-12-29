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
#include "tasmet_utils.h"
#include "tasmet_assert.h"
#include "tasmet_exception.h"
#include "tasmet_constants.h"
#include "duct.h"
#include "ductbc.h"

TaSystem::TaSystem(const pb::System& sys):
    GlobalConf(sys.nf(),sys.freq())
{
    TRACE(14,"TaSystem::TaSystem()");

    if(sys.systemtype() != pb::TaSystem) {
        throw TaSMETError("Invalid system type for TaSystem");
    }

    // Checking parameters T0 and p0 also happens in newGas method
    _gas = std::unique_ptr<Gas>(Gas::newGas(sys.gastype(),sys.t0(),sys.p0()));
    if(!_gas) throw  TaSMETBadAlloc();

    // Create all ducts
    for(const auto& d : sys.ducts()) {
        // d.first: id
        // d.second: duct description
        try {
            _segs[d.first] = new Duct(d.first,d.second);
            if(!_segs[d.first]) throw TaSMETBadAlloc();
        }
        catch(TaSMETError e) {
            // Cleanup the already successfully created Ducts and rethrow
            cleanup();
            throw e;
        }
    }
    // Create all ducts
    for(const auto& d : sys.ductbcs()) {
        // d.first: id
        // d.second: duct description
        try {
            _segs[d.first] = DuctBc::newDuctBc(d.first,
                                               *this,
                                               d.second);

            if(!_segs[d.first]) throw TaSMETBadAlloc();
        }
        catch(TaSMETError e) {
            // Cleanup the already successfully created Ducts and rethrow
            cleanup();
            throw e;
        }
    }
    
    // Copy solution vector, if valid
    const auto& sol = sys.solution();
    us size = sol.size(), i=0;
    if(size>0) {
        _solution = vd(size);
        for(auto& val: sol) {
            _solution(i) = val;
            i++;
        }
    }

}
TaSystem::TaSystem(const TaSystem& o):
    GlobalConf(o),                 // Share a ptr to the Global conf
    _gas(o._gas->copy())
{
    TRACE(25,"TaSystem::TaSystem(TaSystem&) copy");

    // Take over the pointers to the segments
    _segs = o._segs;

    for(auto& seg: _segs){
        seg.second = seg.second->copy();
        if(!seg.second) throw TaSMETBadAlloc();
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
vd TaSystem::residual() const {
    TRACE(15,"TaSystem::residual()");

    vus neqs = getNEqs();
    us total_neqs = arma::sum(neqs);

    if(total_neqs>constants::maxndofs)      {
        throw TaSMETError("Too many DOFS required."
                          " Problem too large.");
    }

    // This vector of indices stores the last equation number + 1 for
    // each equation set in a Segment
    vus eqsend = arma::cumsum(neqs);

    int arbitrateMassEq = getArbitrateMassEq(neqs);

    // This is the mass in the sytem. Only counted when there is an
    // equation present which arbitrates the mass in the system
    d mass=0;
    
    VARTRACE(25,total_neqs);
    VARTRACE(25,eqsend);

    vd residual(total_neqs);

    us i=0;
    const Segment* seg;
    for(auto seg_: _segs) {

        seg = seg_.second;

        if(i==0) {
            // Put the residual of the segment in the over-all residual
            seg->residual(*this,residual.subvec(0,eqsend(0)-1));
        }
        else {
            seg->residual(*this,residual.subvec(eqsend(i-1),eqsend(i)-1));
        }

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
vd TaSystem::getSolution() const {

    if(_solution.size() == 0) {
        // Create the initial solution from the segments
        // and store it here

        vus ndofs = getNDofs();
        vus dofend = arma::cumsum(ndofs);
        us total_dofs = arma::sum(ndofs);
        vd solution = vd(total_dofs);

        us i=0;
        const Segment* seg;
        for(auto& seg_: _segs) {
            seg = seg_.second;

            if(ndofs(i)>0) {
                if(i==0) {
                    solution.subvec(0,ndofs(0)-1) =
                        seg->initialSolution(*this);
                }
                else {
                    solution.subvec(dofend(i-1),dofend(i)-1) =
                        seg->initialSolution(*this);
                }
                i++;
            }
        }
        return solution;
    } // if the solution did not yet exist

    return _solution;
}
const arma::subview_col<d> TaSystem::getSolution(const us seg_id) const {

    vus ndofs = getNDofs();
    vus dofsend = arma::cumsum(ndofs);

    if(seg_id == 0) {
        return _solution.subvec(0,dofsend(0)-1);
    }
    else {
        return _solution.subvec(dofsend(seg_id-1),dofsend(seg_id)-1);
    }
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
    GlobalConf::show();
    
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
// void TaSystem::resetHarmonics(){
//     for(auto seg: _segs) {
//         seg.second->resetHarmonics();
//     }
// }
dmat TaSystem::showJac(){

    TRACE(15,"TaSystem::showJac()");

    return dmat(jacobian());
}

TaSystem::~TaSystem() {
    TRACE(25,"~TaSystem()");
    cleanup();
}
void TaSystem::cleanup() {
    purge(_segs);
}

//////////////////////////////////////////////////////////////////////
