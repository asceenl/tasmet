// tasmet_exception.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
// Implementation of exception TasMETBadAlloc
//////////////////////////////////////////////////////////////////////

#include "tasmet_exception.h"

const char* TaSMETError::what() const throw() {
    return _msg.c_str();
}
void TaSMETError::setContext(const std::string& ctx) {
    std::string oldmsg = _msg;
    _msg = ctx + ": " + oldmsg;
}
const char*  TaSMETBadAlloc::what() const throw() {
	return "Error: memory allocation failed. "
   		"Please make sure enough memory is available and restart the application";
}


//////////////////////////////////////////////////////////////////////

