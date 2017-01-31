// tasystem.h, created March 19th 2014
// Author: J.A. de Jong

// A system contains one or more (un)connected (thermoacoustic)
// segments. Some segments require boundary conditions, these boundary
// conditions are provided as segments as well.

#pragma once
#ifndef _TASYSTEM_H_
#define _TASYSTEM_H_
#include "system.h"
#include "globalconf.h"
#include "triplets.h"
#include "jacobian.h"
#include "gas.h"
#include "protobuf/system.pb.h"
#include <memory>

class Segment;

// Inherit all global configuration members
class TaSystem : public GradientNonlinearSystem, public GlobalConf {
protected:

    d _mass = -1;
    
    std::map<us,Segment*> _segs;		

    std::unique_ptr<Gas> _gas;

    mutable
    vd _solution;               /**< This column vector stores the
                                   current solution. */

    mutable
    GlobalPositionMapper _solution_dofs; /**< This vector of column vectors
                                                      stores columns of doubles that do
                                                      not own the memory they point
                                                      to. Instead, they use the memory of
                                                      _solution.*/

    
    mutable
    vd _residual;

    mutable
    GlobalPositionMapper _residual_eqs;

    mutable
    Jacobian _jac;              /**< Storing the Jacobian brings the
                                   hugh benefit of only allocating the
                                   memory once. Once this is done, all
                                   values can be updated during each
                                   iteration. */
    
    TaSystem& operator=(const TaSystem& other)=delete;

private:
    /** 
     * Initializes the _solution and _solution_dofs vector. This
     * should be done during construction of the object. The method is
     * called from all constructors. Should be done after construction
     * of the segments.
     *
     */
    void initSolRes();

    TaSystem(const TaSystem& o);
public:

    TaSystem(const pb::System&);



    /** 
     * Returns an empty TaSystem with default GlobalConf
     * parameters. The caller of this function owns the object and is
     * responsible for its destruction.
     *
     *
     * @return the TaSystem instance pointer.
     */
    static TaSystem* testSystem() {
        return new TaSystem(pb::System::default_instance());
    }
    const Gas& gas() const {return *_gas;}
    
    // Set and get the mass in the system. If the mass is not set
    // before initializing, the mass is computed from the segment's
    // intial configuration.
    void setMass(d mass) { _mass = mass; }
    d getMass() const;
    
    virtual ~TaSystem();
    virtual TaSystem* copy() const {return new TaSystem(*this);}

    us nSegments() const {return _segs.size();}

    dmat showJac();
    virtual void show(us detailnr=0);

    void residualJac(ResidualJac&) const;

    vd getSolution() const;

    // Obtain the solution vector for the Segment with given id
    const SegPositionMapper& getSolution(const us seg_id) const;
    
    virtual void updateSolution(const vd& sol);

    // Change Nf in the system, while keeping the results.
    void updateNf(us);

    // Reset amplitude data in higher harmonics
    // void resetHarmonics();

    // void delseg(us n); // Not yet implemented.  Delete a segment
    // from the system (we have to determine how elaborated the API
    // has to be.)
    vus getNDofs() const;	// Compute DOFS in all segments
    vus getNEqs() const;        // Compute Equations in all segments

    const Segment& getSegment(const us id) const {return *_segs.at(id);}

    /** 
     * Store the current results to a HDF5 file. For each segment, a
     * group is created with the name corresponding to the segment
     * ID. Then, the group contents is filled based on what the
     * segment wants to store as result data. *Warning*: overwrites
     * existing files without confirmation!
     *
     * @param filename The filename of the output file
     */
    void exportHDF5(const string& filename) const;

protected:
    virtual int getArbitrateMassEq(const vus& neqs) const;
private:
    void cleanup();
}; // class System
  

#endif /* _TASYSTEM_H_ */

