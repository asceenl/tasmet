// export.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "export.h"
#include "tasmet_exception.h"
#include "tasmet_tracer.h"

// Lite version of the library
#include <hdf5_hl.h>

#if TASMET_FLOAT == 64
#define make_dataset H5LTmake_dataset_double
#elif TASMET_FLOAT == 32
#define make_dataset H5LTmake_dataset_float
#else
#error "Invalid TASMET_FLOAT"
#endif

namespace {
    /** 
     * Set the attributes of a dataset
     *
     * @param data the data from which to take the name, unit, symbol
     * @param dsetname the dataset name to add the attributes to
     * @param hfgroup the group
     */
    void setAttributes(const ExportData& data,
                       const string& dsetname,
                       const hid_t hfgroup) {
    
        H5LTset_attribute_string(hfgroup,
                                 dsetname.c_str(),
                                 "name",
                                 data.name.c_str());

         H5LTset_attribute_string(hfgroup,
                                  dsetname.c_str(),
                                  "unit",
                                  data.unit.c_str());

         H5LTset_attribute_string(hfgroup,
                                  dsetname.c_str(),
                                  "symbol",
                                  data.symbol.c_str());


    }
}

void PointData::exportHDF5(hid_t hfgroup) {

    // We use the lite API for setting the data
    
    hsize_t dims[] = {1};         // First dimension has lenght 1
    hsize_t rank = 1;
    herr_t status;

    string dsetname = (string("/") + symbol); // name 

    status = make_dataset(hfgroup,                        // Group
                          dsetname.c_str(),
                          rank,
                          dims,
                          &x);


    setAttributes(*this,dsetname,hfgroup);


}
void TimeData::exportHDF5(hid_t hfgroup) {

    // We use the lite API for setting the data
    
    hsize_t dims[] = {x.size()};         // First dimension has lenght 1
    hsize_t rank = 1;
    
    string dsetname = (string("/") + symbol); // name 
    herr_t status;
    status = make_dataset(hfgroup, // Group
                          dsetname.c_str(), // Dataset name
                          rank,
                          dims,
                          x.memptr());


    setAttributes(*this,dsetname,hfgroup);

}
void PosData::exportHDF5(hid_t hfgroup) {

    TRACE(15,"PosData::exportHDF5");

    // We use the lite API for setting the data
    
    hsize_t dims[] = {x.size()};         // First dimension has lenght 1
    hsize_t rank = 1;
    

    string dsetname = (string("/") + symbol); // name 
    herr_t status;
    status = make_dataset(hfgroup,                        // Group
                          dsetname.c_str(),
                          rank,
                          dims,
                          x.memptr());


    setAttributes(*this,dsetname,hfgroup);

    H5Eprint(status,0);

}
void TXData::exportHDF5(hid_t hfgroup) {

    // We use the lite API for setting the data

    // HDF5 Stores data in row-major ordering. Armadillo uses
    // column-major ordering. Therefore, we store the transpose of the data.
    dmat x = this->x.t();
    
    hsize_t dims[] = {x.n_cols,x.n_rows};         // First dimension has lenght 1
    hsize_t rank = 2;

    d* xptr = x.memptr();

    std::vector<d*> col_ptrs(x.n_cols);
    
    for(us col=0;col<x.n_cols;col++) {
        col_ptrs[col] = &xptr[col*x.n_rows];
    }
    
    string dsetname = (string("/") + symbol); // name
    
    make_dataset(hfgroup, // Group
                 dsetname.c_str(), // name 
                 rank,
                 dims,
                 &col_ptrs[0][0]);

    setAttributes(*this,
                  dsetname,
                  hfgroup);
}



//////////////////////////////////////////////////////////////////////

