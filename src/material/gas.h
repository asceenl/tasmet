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



class Gas{
public:
    DECLARE_ENUM(GasType,air,helium,nitrogen);
private:    
    GasType _gastype;
protected:
    Gas(GasType gastype):_gastype(gastype){}
public:



    Gas(const Gas& other) =delete;
    Gas& operator=(const Gas&)=delete;
    virtual ~Gas(){}

    // Static method to generate a Gas
    static Gas* newGas(const GasType gastype);
    operator GasType() { return _gastype;}

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

    // Adiabatic speed of sound [m/s]
    virtual d cm(d T,d p) const=0;
    virtual vd cm(const vd& T,const vd& p) const=0;

    // Dynamic viscosity [Pa*s]
    virtual d mu(d T,d p) const=0;
    virtual vd mu(const vd& T,const vd& p) const=0;

    // Thermal conductivity [W/mK]
    virtual d kappa(d T,d p) const=0;
    virtual vd kappa(const vd& T,const vd& p) const=0;









};



#endif // GAS_H
//////////////////////////////////////////////////////////////////////
