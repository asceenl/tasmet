// gas.h
//
// Author: J.A. de Jong 
//
// Description:
// Gas properties interface
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAS_H
#define GAS_H
#include <string>
#include <armadillo>
#include "tasmet_enum.h"
#include "tasmet_types.h"
#include "tasmet_constants.h"


class Gas{
public:
    DECLARE_ENUM(GasType,air,helium,nitrogen);
private:    
    GasType _gastype;
protected:
    d _T0,_p0;			/* Reference temperature and pressure */
    
    Gas(GasType gastype,d T0,d p0):_gastype(gastype),_T0(T0),_p0(p0){}
public:
    Gas(const Gas& other) =delete;
    Gas& operator=(const Gas&)=delete;
    virtual ~Gas(){}

    // Static method to generate a Gas


    static Gas* newGas(const GasType gastype,d T0=constants::T0,
                     d p0=constants::p0);

    operator GasType() { return _gastype;}

    d T0() const {return _T0;}
    d p0() const {return _p0;}

    // Speed of sound at the reference temperature and pressure
    d c0() const {return cm(_T0,_p0);}
    d rho0() const {return rho(_T0,_p0);}
    d deltanu0(d freq) const{ return sqrt(2*mu(_T0,_p0)/(rho0()*2*number_pi*freq));}
    d deltanu(d freq,d T,d p) const { return sqrt(2*mu(T,p)/(rho(T,p)*2*number_pi*freq)); }

    // Dimensionless numbers:

    // Specific heat ratio
    d gamma(d T,d p) const {return cp(T,p)/cv(T,p);}
    vd gamma(const vd& T,const vd& p) const {return cp(T,p)/cv(T,p);}

    // Prandtl number
    vd pr(const vd& T,const vd& p) const {return mu(T,p)%cp(T,p)/kappa(T,p);}
    d pr(d T,d p) const {return mu(T,p)*cp(T,p)/kappa(T,p);}

    // Virtuals that are part of the interface
    // Density [kg/m^3]
    virtual d rho(d T,d p) const=0;
    virtual vd rho(const vd& T,const vd& p) const=0;

    // Adiabatic speed of sound
    virtual d cm(d T,d p) const=0;
    virtual vd cm(const vd& T,const vd& p) const=0;    

    // Internal energy [J/kg]
    virtual d e(d T,d p) const=0;
    virtual vd e(const vd& T,const vd& p) const=0;

    // Static enthalpy [J/kg]
    virtual d h(d T,d p) const=0;
    virtual vd h(const vd& T,const vd& p) const=0;

    // Specific heat at constant pressure
    virtual d cp(d T,d p) const=0;
    virtual vd cp(const vd& T,const vd& p) const=0;

    // Specific heat at constant density
    virtual d cv(d T,d p) const=0;
    virtual vd cv(const vd& T,const vd& p) const=0;

    virtual d beta(d T,d p) const=0;
    virtual vd beta(const vd& T,const vd& p) const=0;

    // Dynamic viscosity [Pa*s]
    virtual d mu(d T,d p) const=0;
    virtual vd mu(const vd& T,const vd& p) const=0;

    // Thermal conductivity [W/mK]
    virtual d kappa(d T,d p) const=0;
    virtual vd kappa(const vd& T,const vd& p) const=0;









};



#endif // GAS_H
//////////////////////////////////////////////////////////////////////
