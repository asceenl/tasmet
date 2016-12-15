// consolecolors.h
//
// Author: J.A. de Jong 
//
// Description:
// Print text from C++ to stdout in color
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CONSOLECOLORS_H
#define CONSOLECOLORS_H
#include <iostream>

#define RED_COLOR "\e[31m"
#define GREEN_COLOR "\e[32m"
#define DEFAULT_COLOR " \e[39m"

// Command to clear the content of a console
inline void clearConsole(){
  std::cout << "\033c" << std::endl;
}

#endif // CONSOLECOLORS_H
//////////////////////////////////////////////////////////////////////

