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

DECLARE_ENUM(FileType,FileTypeTXT)

struct ExportData
{
    string name;                /**< Full name of the quantity */
    string unit;                /**< Unit of the quantity */
    string symbol;              /**< Used symbol / abbreviation */

    /** 
     * Export the data to a text file
     *
     * @param FileType 
     * @param filename 
     * @param group 
     *
     * 
     */
    // void export(const string& filename)=0;

    /** 
     * Export the result to an existing HDF5 
     *
     * @param hfgroup 
     */
    #ifdef TASMET_HDF5
    virtual void exportHDF5(hid_t hfgroup)=0;
    #endif


    // void show() const = 0;
    
    virtual ~ExportData(){}
};

/**
 * A physical quantity result, not a function of time or position
 * 
 */
struct PointData: public ExportData {
    d x;                        /**< The value */

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
    vd t;                       /**< The time instances of the
                                   record */
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
    vd X;                       /**< The positions of the
                                   record */
    vd x;                       /**< The quantity as function of the
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

    vd t; /**< The time instances of the
             record */
    vd X; /**< The positions of the record */
    
    dmat x; /**< The quantity at the time and position instance. Note:
               the first axis denotes time, the second axis denotes
               position! */

    #ifdef TASMET_HDF5
    void exportHDF5(hid_t hfgroup);
    #endif
};

#endif // EXPORT_H
//////////////////////////////////////////////////////////////////////
