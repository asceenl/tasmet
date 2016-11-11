// tasmet_exception.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Implementation of exception TasMETBadAlloc
//////////////////////////////////////////////////////////////////////

#include "tasmet_exception.h"

const char*  TasMETBadAlloc::what() const throw() {
	return "Error: memory allocation failed. "
   		"Please make sure enough memory is available and restart the application";
}


//////////////////////////////////////////////////////////////////////

