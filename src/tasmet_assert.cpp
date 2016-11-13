// tasmet_assert.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "tasmet_assert.h"

#if TASMET_DEBUG == 1
#include <cassert>

void tasmet_assertfailed(const char* filename,size_t linenr,const char* statement) {
    cout << "ASSERT: file " << filename << ", line " << linenr <<". " << statement << endl;
    assert(false);
}
#endif
    
    

//////////////////////////////////////////////////////////////////////
