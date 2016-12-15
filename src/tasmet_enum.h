// tasmet_enum.h
//
// Author: J.A. de Jong 
//
// Description: Copied and adapted code from
// http://stackoverflow.com/questions/5093460/how-to-
// convert-an-enum-type-variable-to-a-string

// Used to automatically couple a enum to the describing string.
// Use it as follows:
// 
// #include "tasmet_enum.h"
// DECLARE_ENUM(OS_type, Windows, Linux, Apple)

// void main() {
//     cout << OS_typeToString(Windows) << endl;
//     if (StringToOS_type("Linux") == Linux) { cout << "yay!" << endl; }
//     cout << "Max: " << MAX_NUMBER_OF_OS_type << endl;
// }

#pragma once
#ifndef TASMET_ENUM_H
#define TASMET_ENUM_H
#include <string>
#include <sstream>

// Search and remove whitespace from both ends of the string
inline std::string TrimEnumString(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it)) { it++; }
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit)) { rit++; }
    return std::string(it, rit.base());
}
inline void SplitEnumArgs(const char* szArgs, std::string Array[], int nMax)
{
    std::stringstream ss(szArgs);
    std::string strSub;
    int nIdx = 0;
    while (ss.good() && (nIdx < nMax)) {
        getline(ss, strSub, ',');
        Array[nIdx] = TrimEnumString(strSub);
        nIdx++;
    }
};
#define DECLARE_ENUM(ename, ...) \
    enum ename { __VA_ARGS__, MAX_NUMBER_OF_##ename }; \
    const ename ename##_vec[] = { __VA_ARGS__ };\
    \
    static std::string ename##Strings[MAX_NUMBER_OF_##ename]; \
    inline const char* ename##ToString(ename e) { \
        if (ename##Strings[0].empty()) { SplitEnumArgs(#__VA_ARGS__, ename##Strings, MAX_NUMBER_OF_##ename); } \
        return ename##Strings[e].c_str(); \
    } \
    inline ename StringTo##ename(const char* szEnum) { \
        for (int i = 0; i < MAX_NUMBER_OF_##ename; i++) { if (ename##Strings[i] == szEnum) { return (ename)i; } } \
        return MAX_NUMBER_OF_##ename; \
    }


#endif // TASMET_ENUM_H
//////////////////////////////////////////////////////////////////////
