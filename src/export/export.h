// export.h
//
// Author: J.A. de Jong 
//
// Description:
// Data structure used for exporting
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef EXPORT_H
#define EXPORT_H

#define TASMET_HDF5

#include "tasmet_enum.h"
#include "tasmet_types.h"
#ifdef TASMET_HDF5
#include <hdf5.h>
#endif

struct ExportData
{
    string name;                /**< Full name of the quantity */
    string unit;                /**< Unit of the quantity */
    string symbol;              /**< Used symbol / abbreviation */
    string datatype;            /**< Key representing the datatype */
    /** 
     * Export the result to an existing HDF5 
     *
     * @param hfgroup 
     */
    #ifdef TASMET_HDF5
    virtual void exportHDF5(hid_t hfgroup)=0;
    #endif

    virtual ~ExportData(){}
};

/**
 * A physical quantity result, not a function of time or position
 * 
 */
struct PointData: public ExportData {
    d x;                        /**< The value */
    PointData() { datatype = "point";}
    #ifdef TASMET_HDF5
    void exportHDF5(hid_t hfgroup);
    #endif
};

/**
 * A physical quantity at a single position, as a function of time.
 * 
 */
struct TimeData: public ExportData
{
    TimeData() { datatype = "time";}

    vd x;                       /**< The quantity at the time
                                   instance */

    #ifdef TASMET_HDF5
    void exportHDF5(hid_t hfgroup);
    #endif
};

/**
 * A physical quantity at a single time instance, as a function of
 * position.
 * 
 */
struct PosData: public ExportData
{
    PosData() { datatype = "pos"; }
    vd x; /**< The quantity as function of the
             position */

    #ifdef TASMET_HDF5
    void exportHDF5(hid_t hfgroup);
    #endif
};


/**
 * A physical quantity at a single position, as a function of time and
 * position (X)
 * 
 */
struct TXData : public ExportData
{
    TXData() { datatype = "postime"; }

    dmat x; /**< The quantity at the time and position instance. Note:
               the first axis denotes time, the second axis denotes
               position! */

    #ifdef TASMET_HDF5
    void exportHDF5(hid_t hfgroup);
    #endif
};

#endif // EXPORT_H
//////////////////////////////////////////////////////////////////////
