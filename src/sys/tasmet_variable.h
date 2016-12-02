// tasmet_variable.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_VARIABLE_H
#define TASMET_VARIABLE_H

#include "globalconf.h"
#include <tasmet_types.h>
#include <tasmet_assert.h>

class Variable;			// Forward declaration
Variable operator*(const double&,const Variable&);
// Variable operator+(const d&,const Variable&);

class Variable {

    friend Variable operator*(const double&,const Variable&);

    int _dofnr=-1;
    vd _tdata,_adata;

    gc_ptr _gc;

public:
    void setDofNr(us Dofnr){_dofnr=Dofnr;}
    int getDofNr() const{return _dofnr;}

    Variable(const gc_ptr&,double); // Initialize with one time-average value
    Variable(const Variable& o);
    Variable(const gc_ptr&);	// Initialize with zeros

    // Initialize with amplitudedata. With _tdata if adata is set to false
    Variable(const gc_ptr&,const vd& data,bool adata=true);

    // Assign with frequency domain data
    Variable(const gc_ptr&,const vc& data);
    
    Variable& operator=(const Variable&);
      
    ~Variable(){}

    void setGc(const gc_ptr&);
    const gc_ptr& getGc() const {return _gc;}

    void updateNf();
    
    // Get methods
    d operator()(us i) const; // Extract amplitude data result at
                              // specific frequency

    operator vd() const {return _adata;}

    // Extract data
    const vd& tdata() const  {return _tdata; } //Get time data
    const vd& adata() const  {return _adata; } //Get amplitude data                                                 //vector

    // Obtain a time response vector
    vd timeResponse(us nperiod=2,us ninst=100) const;
    // Obtain the corresponding time vector
    vd timeResponseTime(us nperiod=2,us ninst=100) const;

    dmat diagt() const {return diagmat(_tdata);}
    dmat diag() const {return diagmat(_adata);}    

    //Set/reset methods
    void resetHarmonics();
    void updateNDofs(us new_ndofs);

    void setadata(const vd& values); //Set amplitude data vector to these values
    void settdata(double value); //Set time data to specific value for all time
    void settdata(const vd& values);
    void setadata(us freq,double val); //Set result vector at specific frequency
    void setadata(const vc& values); //Set result vector to these values,

    us size() const {return _adata.size();}
    // Specific methods to the result using time domain data
    // Operations ********************

    Variable ddt() const;       // Time-derivative of this variable
    
    Variable operator/(const Variable& Variable2) const; // Time-domain division operator
    Variable operator/(d value) const; // Time-domain or frequency domain division operator

    // Multiply two Variables in time domain
    Variable operator*(const Variable& tasystem) const;

    // Multiply a tasystem with a scalar. This operation is possible
    // for both frequency and time domain data
    Variable operator*(d scalar) const;

    // add two Variableiables
    Variable operator+(const Variable& other) const;
    //Subtract two Variableiables
    Variable operator-(const Variable& Variable2) const;
    // with Note multiplication is defined outside of the class

    // If we need to multiply two numbers in frequency domain, this
    // corresponds to a matrix-vector multiplication (cosines and
    // sines) are mixed up due to the complex numbers. This product
    // can be obtained by getting the matrix-Variable of the first
    // Variable. The following function will give the effective matrix
    dmat freqMultiplyMat() const;

    // Get the result vector in complex form
    vc getcRes() const;
    
};


#endif /* TASMET_VARIABLE_H_ */

