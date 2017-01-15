// adiabatictemp.h
//
// Author: J.A. de Jong 
//
// Description:
// 
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ADIABATICTEMP_H
#define ADIABATICTEMP_H
#include "tasmet_types.h"
class PerfectGas;
/** 
 * Compute the adiabatic/isentropic temperature as a function of the pressure
 * for a thermally perfect gas.
 *
 * @param gas: The gas to compute the temperature for. Be aware of the
 * value for T0 and p0 in the Gas.
 * @param pressure the pressure to compute the temperature for
 *
 * @return the temperature
 */
vd adiabaticTemp(const PerfectGas& gas,const vd& pressure);

#endif // ADIABATICTEMP_H
//////////////////////////////////////////////////////////////////////
