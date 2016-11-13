// tasmet_assert.h
//
// Author: J.A. de Jong 
//
// Description:
// Implementation of asserts
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_ASSERT_H
#define TASMET_ASSERT_H

#ifndef TASMET_DEBUG
#include <type_traits>
static_assert(false, "TASMET_DEBUG macro not defined. Please set it to 1 or 0");
#endif

#if TASMET_DEBUG == 1
#include <cassert>
#include "tasmet_io.h"

void tasmet_assertfailed(const char* filename,size_t linenr,const char* statement);

#define tasmet_assert(assertion,txt) \
    if (!(assertion))                                                \
        {                                                            \
            tasmet_assertfailed(__FILE__, __LINE__, txt );            \
        }                                                             \
    }                                                                 \

#else
#define tasmet_assert(assertion,txt)
#endif



#endif // TASMET_ASSERT_H
//////////////////////////////////////////////////////////////////////
