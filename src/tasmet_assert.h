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
#include <type_traits>
#ifndef TASMET_DEBUG

static_assert(false, "TASMET_DEBUG macro not defined. Please set it to 1 or 0");

#endif

// Compile-time type checking for template instantiation.
template<class T, class U>
struct is_same : std::false_type {};
 
template<class T>
struct is_same<T, T> : std::true_type {};


#if TASMET_DEBUG == 1
#include <cassert>
#include "tasmet_io.h"

void tasmet_assertfailed(const char* filename,size_t linenr,const char* statement);

#define tasmet_assert(assertion,txt)                                  \
{                                                                     \
    if (!(assertion))                                                 \
        {                                                             \
            tasmet_assertfailed(__FILE__, __LINE__, txt );            \
        }                                                             \
}                                                                     

#else
#define tasmet_assert(assertion,txt)
#endif



#endif // TASMET_ASSERT_H
//////////////////////////////////////////////////////////////////////
