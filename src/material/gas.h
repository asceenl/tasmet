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

#include "protobuf/gas.pb.h"

using pb::GasType;
using pb::air;
using pb::helium;
using pb::nitrogen;

#define element_wise(function) \
    vd function_(T.size());     \
    for(us i=0;i<T.size();i++) \
        function_(i) = function(T(i),p(i));     \
    return function_



class Gas{
public:

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
    virtual Gas* copy() const = 0;

    static Gas* newGas(const GasType gastype,d T0, d p0);

    operator GasType() { return _gastype;}
    const string& gasName() const { return pb::GasType_Name(_gastype); }
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
    vd gamma(const vd& T,const vd& p) const {
        element_wise(gamma);
    }

    // Prandtl number
    vd pr(const vd& T,const vd& p) const {return mu(T,p)%cp(T,p)/kappa(T,p);}
    d pr(d T,d p) const {return mu(T,p)*cp(T,p)/kappa(T,p);}

    // Virtuals that are part of the interface
    // Density [kg/m^3]
    virtual d rho(d T,d p) const=0;
    vd rho(const vd& T,const vd& p) const {
        element_wise(rho);
    }

    virtual d drhodT(d T,d p) const=0;
    vd drhodT(const vd& T,const vd& p) const {
        element_wise(drhodT);
    }

    virtual d drhodp(d T,d p) const=0;
    vd drhodp(const vd& T,const vd& p) const {
        element_wise(drhodp);
    }

    // Adiabatic speed of sound
    virtual d cm(d T,d p) const=0;
    vd cm(const vd& T,const vd& p) const {
        element_wise(cm);
    }

    // Internal energy [J/kg]
    virtual d e(d T,d p) const=0;
    vd e(const vd& T,const vd& p) const {
        element_wise(e);
    }

    // Static enthalpy [J/kg]
    virtual d h(d T,d p) const=0;
    vd h(const vd& T,const vd& p) const {
        element_wise(h);
    }

    // Specific heat at constant pressure
    virtual d cp(d T,d p) const=0;
    vd cp(const vd& T,const vd& p) const {
        element_wise(cp);
    }

    // Specific heat at constant density
    virtual d cv(d T,d p) const=0;
    vd cv(const vd& T,const vd& p) const {
        element_wise(cv);
    }

    // Dynamic viscosity [Pa*s]
    virtual d mu(d T,d p) const=0;
    vd mu(const vd& T,const vd& p) const {
        element_wise(mu);
    }

    // Thermal conductivity [W/mK]
    virtual d kappa(d T,d p) const=0;
    vd kappa(const vd& T,const vd& p) const {
        element_wise(kappa);
    }

};

#undef element_wise


#endif // GAS_H
//////////////////////////////////////////////////////////////////////
