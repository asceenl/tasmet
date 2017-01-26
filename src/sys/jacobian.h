// jacobian.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JACOBIAN_H
#define JACOBIAN_H
#include "tasmet_types.h"

#include <unordered_map>
#include <map>

class TripletList;
/**
 * A PosId is a specifier for the position in the global Jacobian
 * matrix. Both considering the column as well as the row
 * 
 */
struct PosId {
    us segno;                  /**< This is the segment number */
    us segoffset;              /**< This is the offset in the segment */

    bool operator==(const PosId& other) const {
        return (segno==other.segno) &&
            (segoffset == other.segoffset);
    }

    bool operator<(const PosId& other) const {
        if(other.segno < segno) return true;
        else if(other.segno > segno) return false;
        else return (other.segoffset < segoffset);

    }
};
/**
 * To use the PosId in an unordered_map, it requires a custom hash
 * function. This struct has a custom function to generate a hash from
 * the given two integers.
 * 
 */

struct hash_PosId
{
    std::size_t operator() ( const PosId& m ) const
    {
        const us a = m.segno+1 ;
        const us b = m.segoffset+1 ;
        return std::hash<us>()( a*b + (a+b) ) ;
    }
};


typedef std::unordered_map<PosId,dmat,hash_PosId> JacRow;

typedef std::vector<JacRow> SegJacRows;

typedef std::map<us,SegJacRows> Jacobian;

typedef std::vector<vd*> SegPositionMapper;
typedef std::map<us, SegPositionMapper> GlobalPositionMapper;

#endif // JACOBIAN_H
//////////////////////////////////////////////////////////////////////
