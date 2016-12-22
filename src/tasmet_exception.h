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
    std::string _msg;
public:
    TaSMETError(const std::string& msg = "") :
        std::runtime_error(""),
        _msg(msg)
    {}
    TaSMETError(const std::stringstream& stream) :
        std::runtime_error(""),
        _msg(stream.str())
    {
        
    }
    void setContext(const std::string& ctx);
    virtual const char* what() const throw ();
};

class TaSMETBadAlloc: public std::bad_alloc {
	virtual const char* what() const throw();
};

#endif // TASMET_EXCEPTION_H
//////////////////////////////////////////////////////////////////////

