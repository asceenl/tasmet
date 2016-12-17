// tasystem.h, created March 19th 2014
// Author: J.A. de Jong

// A system contains one or more (un)connected (thermoacoustic)
// segments. Some segments require boundary conditions, these boundary
// conditions are provided as segments as well.

#pragma once
#ifndef _TASYSTEM_H_
#define _TASYSTEM_H_
#include "system.h"
#include "segment.h"
#include "globalconf.h"
#include "triplets.h"
#include "gas.h"
#include <map>

// Inherit all global configuration members
class TaSystem : public GradientNonlinearSystem {
protected:

    d _mass = -1;
    
    std::map<us,Segment*> _segs;		

    gc_ptr _gc;

    Gas* _gas = nullptr;
    
    TaSystem& operator=(const TaSystem& other)=delete;
    TaSystem(const TaSystem& o);
public:
    TaSystem(const GlobalConf& gc,const Gas& g);

    // Set globalconf configuration. Applies updateNf as well.
    void setGc(const GlobalConf& gc);
    const Gas& getGas() const {return *_gas;}
    void setGas(const Gas& g);
    
    // Set and get the mass in the system. If the mass is not set
    // before initializing, the mass is computed from the segment's
    // intial configuration.
    void setMass(d mass) { _mass = mass; }
    d getMass() const;
    
    virtual ~TaSystem();
    virtual TaSystem* copy() const {return new TaSystem(*this);}

    us nSegments() const {return _segs.size();}

    TaSystem& add(const us id,const Segment& s);	// Add a segment to the
    // system. It creates a copy

    dmat showJac();
    virtual void show(us detailnr=0);

    vd residual() const; 

    vd getSolution() const;
    
    virtual void updateSolution(const vd& resvec);	// Update the solution

    // Compute Jacobian matrix. The dampfac value is used in an
    // EngineSystem
    sdmat jacobian() const;		// Return Jacobian matrix

    // Change Nf in the system, while keeping the results.
    void updateNf(us);

    // Reset amplitude data in higher harmonics
    void resetHarmonics();

    // void delseg(us n); // Not yet implemented.  Delete a segment
    // from the system (we have to determine how elaborated the API
    // has to be.)
    vus getNDofs() const;	// Compute DOFS in all segments
    vus getNEqs() const;        // Compute Equations in all segments

    const Segment& getSegment(const us id) const {return *_segs.at(id);}

protected:
    virtual int getArbitrateMassEq(const vus& neqs) const;
    virtual TripletList jacTriplets() const;

}; // class System
  

#endif /* _TASYSTEM_H_ */

