// tasmet_constants.h
//
// Author: J.A. de Jong 
//
// Description:
// Definition of code constants
#pragma once
#ifndef TASMET_CONSTANTS_H
#define TASMET_CONSTANTS_H
#include "tasmet_enum.h"
#include "tasmet_types.h"

template<typename T>
T max(T& t1,T& t2) { return t1>t2?t1:t2;}
template<typename T>
T min(T& t1,T& t2) { return t1>t2?t2:t1;}

// Variables and their names
// Unfortunately to let the code compile with Swig v 2.0, strongly
// typed enums are not supported. Therefore this is a normal
// enumerated type and not an enum class.
DECLARE_ENUM(Varnr,
             none,                         // None
             rho,                        // Density
             m,                          // Mass flow (rho*U)
             T,                          // Temperature
             p,                          // Pressure
             Ts,                         // Temperature of the solid
             Tw,                         // Temperature of the solid wall
             mH,                         // Enthalpy flow (Watts)
             U,                          // Volume flow (m^3/s)
             u,                          // Velocity (U/Sf)
             mu,                         // Momentum flux
             Q,                          // Heat flow
             Qs,                 // Solid heat Flow
             F,                 // A mechanical domain force [N]
             x,                // A mechanical displacement [m]
             Z,                 // A mechanical impedance [N/m]
             mEkin	       // Kinetic energy flow (Watts)
             );

DECLARE_ENUM(Pos,posleft,posright);

DECLARE_ENUM(EqType,
             Con,			// Continuity
             Mom,			// Momentum
             Ene,			// Energy-like equation
             Ise,			// Isentropic
             Sta,			// State
             Sol,			// SolidEnergy
             SolTwEq,			// Solid wall temperature equation
             Mu_is_m_u,      // momentumflow is massflow_squared div
             // density*cs_area
             BcEqP,
             BcEqu,
             BcEqStateBc
             );



namespace constants {

    typedef unsigned us;

    const us min_ngp=4;              // Minimum number of gridpoints
    const us max_ngp=3000;           // Maximum number of gridpoints
    const us default_ngp = 100;
    
    const us maxNf=100;              // Maximum number of frequencies
    const d minomg=1e-3;            // Minimal oscillation frequency
    const d maxomg=1e5;

    const int maxsegs=30;           // Maximum number of segments in a TaSystem
    const int maxndofs=600000;      // Maximum number of DOFS


    const d min_p0=1e0;
    const d max_p0=1e7;
    const d default_p0=101325;            // Reference pressure [Pa]

    const d min_T0 = 2;                 // Minimal temperature
    const d max_T0 = 2000;              // Maximal temperature
    const d default_T0=293.15;            // Reference temperature [K]



    const d min_funtol = 1e-16;
    const d default_funtol = 1e-6;
    const d max_funtol = 1e-1;
    const us funtol_decimals = 10;

    const us default_maxiter = 1000;
    const us min_maxiter = 1;
    const us max_maxiter = 100000000;

    const us field_decimals = 10;
    
    const d default_L = 1.0;
    const d min_L = 1e-9;
    const d max_L = 1e6;

    // These variable numbers are important, as they determine the
    // position of these variables in the array in cell.h
    // const int rho=1;
    // const int m=2;
    // const int T=3;
    // const int p=4;
    // const int Ts=5;
    // Number of variables
    const int nvars_reserve=7;
    const int neqs_reserve=7;
  
} // namespace constants

#endif // TASMET_CONSTANTS_H
