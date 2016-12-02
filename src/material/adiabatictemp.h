// adiabatictemp.h
//
// Author: J.A. de Jong 
//
// Description:
// Compute the adiabatic/isentropic temperature as a function of the pressure
// for a thermally perfect gas
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ADIABATICTEMP_H
#define ADIABATICTEMP_H

class PerfectGas;
class Variable;

Variable adiabaticTemp(const PerfectGas& gas,const Variable& pressure);

#endif // ADIABATICTEMP_H
//////////////////////////////////////////////////////////////////////
