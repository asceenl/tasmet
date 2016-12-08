// tasmet_exception.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_EXCEPTION_H
#define TASMET_EXCEPTION_H

#include <string>
#include <stdexcept>
#include <sstream>              // stringstream

class TaSMETError : public std::runtime_error {
public:
    TaSMETError(const std::string& msg = "") : std::runtime_error(msg) {}
    TaSMETError(const std::stringstream& stream) : std::runtime_error(stream.str()){}
};

class TasMETBadAlloc: public std::bad_alloc {
	virtual const char* what() const throw();
};

#endif // TASMET_EXCEPTION_H
//////////////////////////////////////////////////////////////////////

