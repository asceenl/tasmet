#pragma once
#ifndef SOLID_H
#define SOLID_H

#include "tasmet_types.h"
#include "tasmet_enum.h"


class Solid{
public:
    DECLARE_ENUM(SolidType,stainless,stainless_hopkins,copper,kapton);
private:
    SolidType _solidtype;
protected:
    Solid(const SolidType);
public:
    Solid(const Solid&) = delete;
    Solid& operator=(const Solid&) = delete;
    operator SolidType() const {return _solidtype;}

    Solid* newSolid(const SolidType);

    virtual vd kappa(const vd& T) const=0;
    virtual d kappa(const d& T) const=0;
    virtual vd c(const vd& T) const=0;
    virtual d c(const d& T) const=0;
    virtual vd rho(const vd& T) const=0;
    virtual d rho(const d& T) const=0;
    virtual ~Solid();

};

class StainlessHopkins:public Solid
{
public:
    StainlessHopkins(): Solid(stainless_hopkins){}
    vd kappa(const vd& T) const;
    d kappa(const d& T) const;
    vd c(const vd& T) const;
    d c(const d& T) const;
    vd rho(const vd& T) const;
    d rho(const d& T) const;
};

class Stainless: public Solid{
public:
    Stainless(): Solid(stainless){}
    vd kappa(const vd& T) const;
    d kappa(const d& T) const;
    vd c(const vd& T) const;
    d c(const d& T) const;
    vd rho(const vd& T) const;
    d rho(const d& T) const;

};
class Copper: public Solid{
public:
    Copper(): Solid(copper){}
    vd kappa(const vd& T) const;
    d kappa(const d& T) const;
    vd c(const vd& T) const;
    d c(const d& T) const;
    vd rho(const vd& T) const;
    d rho(const d& T) const;
};

class Kapton: public Solid {
public:
    Kapton():Solid(kapton){}
    vd kappa(const vd& T) const { return 0.2*(1.0-exp(-T/100.0));}
    d kappa(const d& T) const { return 0.2*(1.0-exp(-T/100.0));}
    vd c(const vd& T) const {return 3.64*T;}
    d c(const d& T) const {return 3.64*T;}
    vd rho(const vd& T) const {return 1445.0-0.085*T;}
    d rho(const d& T) const {return 1445.0-0.085*T;}
};

#endif /* SOLID_H */
