// segment.h
//
// Author: J.A. de Jong 
//
// Description: The Segments class is the base class for
// both the normal segments and the connectors. The main difference
// between a segment and a connector is, that a connector only provide
// equations. A normal segment contains both. All
// common stuff, such as a name, initialization
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SEGMENT_H
#define SEGMENT_H
#include <hdf5.h>
#include "tasmet_types.h"
#include "tasmet_exception.h"
#include "jacobian.h"
// #include "phaseconstraint.h"

class GlobalConf;
class TaSystem;

class Segment{

protected:

    const TaSystem& sys;

    // ID
    us _id;
    // Name
    std::string _name;

    Segment(const TaSystem& sys,
            const us id,
            const std::string& name): sys(sys),_id(id),_name(name) {}
    Segment(const TaSystem& sys,const Segment& o):
        Segment(sys,o._id,o._name){}
    Segment& operator=(const Segment&)=delete;
public:
    virtual ~Segment(){}

    /** 
     * Returns a copy of the current segment, coupled to the TaSystem
     * as given function parameter.
     *
     * @param sys: The new TaSystem to which this segment should be 
     * coupled.
     * @return the Segment copy
     */    
    virtual Segment* copy(const TaSystem&) const = 0;
    
    virtual void initialSolution(SegPositionMapper&) const = 0;

    virtual void residualJac(SegPositionMapper& residual,
                             SegJacRows& jacrows// Here we store the residual
                             ) const=0;

    // Get and set name
    const std::string& getName() const{return _name;} // This one is just the name
    void setName(const std::string& name){ _name = name; } // This one is just the name
    
    // Tell a TaSystem whether this Segment arbitrates Mass or
    // not. The special return value of -1 tells it does not. If it
    // does, the derived class should return which equation should be
    // overwritten with the mass arbitration equation.
    virtual int arbitrateMassEq() const {return -1;}

    // Return the total number of equations in this segment
    virtual us getNEqs() const { return 0;}
    
    // Return the total number of DOFS in this segment
    virtual us getNDofs() const { return 0;}

    // Return the current mass in this segment
    virtual d getMass() const = 0;
    virtual void dmtotdx(vd& dmtotdx,us dof_start) const {}
    
    virtual void show(us verbosity_level) const=0;

    // Reset amplitude data in higher harmonics
    // virtual void resetHarmonics() = 0;

    virtual void exportHDF5(const hid_t group_id) const {}

};


#endif // SEGMENT_H
//////////////////////////////////////////////////////////////////////
