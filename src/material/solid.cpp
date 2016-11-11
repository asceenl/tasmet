#include "solid.h"
#include "tasmet_tracer.h"

//Stainless steel
//Container class
Solid* Solid::newSolid(const SolidType solidtype){
    switch (solidtype) {
    case stainless: 
        return new Stainless();
        break;
    case kapton:
        return new Kapton();
        break;
    case copper:
        return new Copper();
        break;
    case stainless_hopkins:
        return new StainlessHopkins();
        break;
    default:
        FATAL("Undefined solid type");
        break;
    }
    return nullptr;
}

vd StainlessHopkins::c(const vd& T) const{return 490*pow(T,0);}
d StainlessHopkins::c(const d& T) const{return 490;}   
vd StainlessHopkins::rho(const vd& T) const{return 7900*pow(T,0);}
d StainlessHopkins::rho(const d& T) const{return 7900;}  
vd StainlessHopkins::kappa(const vd& T) const{return 14.9*pow(T,0);}
d StainlessHopkins::kappa(const d& T) const{return 14.9;}   

   
vd Stainless::c(const vd& T) const {
    vd arg=1.7054e-6*pow(T,-0.88962)+22324.0/pow(T,6);
    return pow(arg,-1.0/3.0)+15/T;
}
d Stainless::c(const d& T) const {
    d arg=1.7054e-6*pow(T,-0.88962)+22324.0/pow(T,6);
    return pow(arg,-1.0/3.0)+15/T;
}
vd Stainless::rho(const vd& T) const {
    return 8274.55-1055.23*exp(-1.0*pow((T-273.15-2171.05)/2058.08,2));
}
d Stainless::rho(const d& T) const {
    return 8274.55-1055.23*exp(-1.0*pow((T-273.15-2171.05)/2058.08,2));
}
vd Stainless::kappa(const vd& T) const {
    return pow(266800.0*pow(T,-5.2)+0.21416*pow(T,-1.6),-0.25);
}
d Stainless::kappa(const d& T) const {
    return pow(266800.0*pow(T,-5.2)+0.21416*pow(T,-1.6),-0.25);
}

//Copper
vd Copper::kappa(const vd& T) const {
    return 398.0-0.567*(T-300.0);
}
d Copper::kappa(const d& T) const { return 398.0-0.567*(T-300.0); }
vd Copper::c(const vd& T) const {return 420.0*pow(T,0);}
d Copper::c(const d& T) const {return 420.0*pow(T,0);}
vd Copper::rho(const vd& T) const {return 9000.0*pow(T,0);}
d Copper::rho(const d& T) const {return 9000.0*pow(T,0);}


