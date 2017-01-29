// tasystem.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include <hdf5.h>

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
            _segs[d.first] = new Duct(*this,d.first,d.second);
            if(!_segs[d.first]) throw TaSMETBadAlloc();
        }
        catch(TaSMETError e) {
            // Cleanup the already successfully created Ducts and rethrow
            cleanup();
            throw e;
        }
    }
    // Create all ductbcs
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

    initSolRes();

}
TaSystem::TaSystem(const TaSystem& o):
    GlobalConf(o),                 // Share a ptr to the Global conf
    _gas(o._gas->copy()),
    _solution(o._solution)
{
    TRACE(25,"TaSystem::TaSystem(TaSystem&) copy");

    // Take over the pointers to the segments
    _segs = o._segs;

    for(auto& seg: _segs){
        seg.second = seg.second->copy(*this);
        if(!seg.second) throw TaSMETBadAlloc();
    }

    initSolRes();

}
 void TaSystem::initSolRes() {
     // Create the initial solution from the segments and store it
     // here. Please be careful not to call any virtual functions!
     vus ndofs = getNDofs();
     vus neqs = getNEqs();
     
     us Ns = this->Ns();

     us total_dofs = arma::sum(ndofs);
     us total_eqs = arma::sum(neqs);

     if(total_dofs != total_eqs) {
         throw TaSMETError("Number of equations does not match number of DOFS. "
                           "This is probably due to missing, or too much boundary conditions");
     }

     if(total_dofs*Ns > constants::maxndofs) {
        stringstream error;
        error << "Too many DOFS required. Problem too large. "
            "Number of equations computed: ";
        error << total_dofs*Ns;
        throw TaSMETError(error);
     }


     _solution = zeros(total_dofs*Ns);
     _residual = zeros(total_dofs*Ns)+arma::datum::nan;
    
     d* solptr = _solution.memptr();
     d* resptr = _residual.memptr();
     
     us globalposdof=0;
     us globalposeq=0;

     // Loop over the segments, and for each dof in the segment, we
     // allocate a vd, with its memory pointed to from the global
     // solution pointer.

     for(auto& seg_: _segs) {

         us segid = seg_.first;
         VARTRACE(15,segid);

         for(us i=0;i< ndofs(segid); i++) {
            
             // false: own memory (means the vd does not own the memory
             // true: strict true means bound to the memory for its
             // lifetime. Does not allow memory adjustments.
             _solution_dofs[segid].push_back(new vd(&solptr[globalposdof*Ns],Ns,false,true));
             globalposdof++;
             VARTRACE(15,i);
         }

         // Set the initial solution
         if(ndofs(segid)>0) 
             seg_.second->initialSolution(_solution_dofs.at(segid));



         for(us i=0;i< neqs(segid); i++) {
            
             // false: own memory (means the vd does not own the memory
             // true: strict true means bound to the memory for its
             // lifetime. Does not allow memory adjustments.
             _residual_eqs[segid].push_back(new vd(&resptr[globalposeq*Ns],Ns,false,true));

             _jac[segid].push_back(JacRow());

             globalposeq++;
         }

     }

     tasmet_assert(globalposdof == total_dofs,"Bug in assigning solution vectors");
     tasmet_assert(globalposeq == total_eqs,"Bug in assigning residual vectors");

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
void TaSystem::residualJac(ResidualJac& resjac) const {
    TRACE(15,"TaSystem::residual()");

    int arbitrateMassEq = -1;//getArbitrateMassEq(neqs);

    // This is the mass in the sytem. Only counted when there is an
    // equation present which arbitrates the mass in the system
    d mass=0;

    #ifdef TASMET_DEBUG
    _residual *= arma::datum::nan;
    #endif

    us segid;
    const Segment* seg;
    for(auto seg_: _segs) {
        segid = seg_.first;
        seg = seg_.second;

            // Put the residual of the segment in the over-all residual
        seg->residualJac(_residual_eqs[segid],_jac[segid]);

        // Count the mass, add it
        if(arbitrateMassEq!=-1) {
            mass += seg->getMass();
        }
        
     }

    us Ns = this->Ns();
    vus neqs = getNEqs();
    vus neqs_sum = arma::cumsum(neqs);
    us total_neqs = arma::sum(neqs);

    vus ndofs = getNDofs();
    vus ndofs_sum = arma::cumsum(ndofs);
    us total_ndofs = arma::sum(ndofs);

    // Now we create the triplets
    TripletList triplets(total_neqs*Ns);

    for(auto& segjacrows : _jac) {

        // The current segment number
        us segno = segjacrows.first;

        // The row offset for this segment
        us rowoffset = (segno==0) ? 0 : neqs_sum(segno-1)*Ns;

        for(auto& jacrow: segjacrows.second) {
            
            for(auto& jaccol : jacrow) {
                const PosId& id = jaccol.first;

                us coloffset = (id.segno==0) ? 0 : ndofs_sum(segno-1)*Ns;
                coloffset += id.segoffset;
                coloffset*=Ns;

                dmat& colmat = jaccol.second;
                
                for(us i=0;i<Ns;i++){

                    us row = rowoffset + i;

                    for(us j=0;j<Ns;j++) {

                        us col = coloffset + j;
                        triplets.addTriplet(Triplet(row,col,colmat(i,j)));

                    }

                    
                }
                
                
            } // For loop over the columns

            rowoffset += Ns;

        } // For loop over the rows for this segment

    } // For loop over the segments

    TRACE(15,"Obtained residual from all Segments");

    #ifdef TASMET_DEBUG
    assert(arbitrateMassEq< (int) total_neqs);    
    #endif // TASMET_DEBUG

    // Exchange equation if we need to arbitrate mass
    if(arbitrateMassEq>=0) {
        _residual(arbitrateMassEq)=mass - _mass;
    }

    resjac.jacobian = sdmat(triplets);
    resjac.residual = _residual;

}
vd TaSystem::getSolution() const {
    return _solution;
}
const SegPositionMapper& TaSystem::getSolution(const us seg_id) const {
    return _solution_dofs.at(seg_id);
}
vus TaSystem::getNDofs() const  {
    TRACE(0,"TaSystem::getNDofs()");
    vus Ndofs(_segs.size());
    for (auto seg : _segs) {
        Ndofs(seg.first)=seg.second->getNDofs();
    }
    return Ndofs;
}
vus TaSystem::getNEqs() const  {
    TRACE(15,"TaSystem::getNEqs()");
    vus Neqs(_segs.size());
    for (auto seg :_segs) {
        Neqs(seg.first)=seg.second->getNEqs();
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
            seg.second->show(detailnr);
        }
    } // detailnr>0
}
// TripletList TaSystem::jacTriplets() const {

//     TRACE(14,"TaSystem::jacobian()");
//     vus ndofs=getNDofs();
//     vus neqs=getNEqs();
//     us total_ndofs = arma::sum(ndofs);
//     us total_neqs = arma::sum(neqs);
//     us i=0,dofnr=0,eqnr=0;
//     Jacobian j(total_ndofs);

//     int arbitrateMassEq = getArbitrateMassEq(neqs);
//     vd dmtotdx;
//     if(arbitrateMassEq > -1)
//         dmtotdx = vd(total_ndofs);

//     const Segment* seg;
//     for(auto& seg_ :_segs) {
//         seg = seg_.second;
//         seg->jac(*this,j,dofnr,eqnr);
//         if(arbitrateMassEq > -1) {
//             seg->dmtotdx(*this,dmtotdx,dofnr);
//         }
//         eqnr += neqs(i);
//         dofnr += ndofs(i);
//         i++;
//     }

//     if(dofnr!=eqnr){
//         throw TaSMETError("System of equations is over/underdetermined");
//     }
    
//     // Convert to tripletlist
//     TripletList jac=j;

//     assert(arbitrateMassEq< (int) total_neqs);

//     // Exchange equation if we need to arbitrate mass
//     if(arbitrateMassEq!=-1) {
//         // Replace this equation with global mass conservation
//         jac.zeroOutRow(arbitrateMassEq);

//         us dmtotdxsize=dmtotdx.size();
//         for(us k=0;k<dmtotdxsize;k++)
//             if(dmtotdx(k)!=0){
//                 // TRACE(20,"k: " << k);
//                 // TRACE(20,"dmtotdx:"<< dmtotdx(k));
//                 jac.addTriplet(Triplet(arbitrateMassEq,k,dmtotdx(k)));
//             }
//     }

//     return jac;
// }
// void TaSystem::resetHarmonics(){
//     for(auto seg: _segs) {
//         seg.second->resetHarmonics();
//     }
// }
// dmat TaSystem::showJac(){

//     TRACE(15,"TaSystem::showJac()");

//     return dmat(jacobian());
// }
TaSystem::~TaSystem() {
    TRACE(25,"~TaSystem()");
    cleanup();
}
void TaSystem::cleanup() {

    purge(_segs);
    for(auto& segdofs: _solution_dofs) {
        for(vd* dof : segdofs.second){
            delete dof;
        }
    }
    for(auto& segdofs: _residual_eqs) {
        for(vd* dof : segdofs.second){
            delete dof;
        }
    }

}

void TaSystem::exportHDF5(const string& filename) const {

    TRACE(15,"TaSystem::exportHDF5");
    hid_t file_id;
    herr_t status;

    file_id = H5Fcreate (filename.c_str(),
                         H5F_ACC_TRUNC,
                         H5P_DEFAULT,
                         H5P_DEFAULT);    

    

    for(const auto& seg_ : _segs) {
        // Create a group for each segment
        hid_t grp_id = H5Gcreate(file_id,
                                  (string("/") + std::to_string(seg_.first)).c_str(),
                                  H5P_DEFAULT,
                                  H5P_DEFAULT,
                                  H5P_DEFAULT);

        seg_.second->exportHDF5(grp_id);

        H5Gclose(grp_id);

        // ^^ uncommenting above results in segfault. Why?
        // seg_.second->exportHDF5(file_id);
        
    }

    status = H5Fclose(file_id);
    H5Eprint(status,0);


}                              



//////////////////////////////////////////////////////////////////////
