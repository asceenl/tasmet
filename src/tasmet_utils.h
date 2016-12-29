// tasmet_utils.h
//
// Author: J.A. de Jong 
//
// Description:
// Some generic utils.
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_UTILS_H
#define TASMET_UTILS_H

// Purge a vector of components
template<typename T>
void purge(std::vector<T>& vec){
    for (T& it: vec){
        delete it;
        it=nullptr;
    }
    vec.clear();
}

// Purge a vector of components
template<typename Key,typename T>
void purge(std::map<Key,T>& map){
    for (auto& it: map){
        delete it.second;
        it.second=nullptr;
    }
    map.clear();
}
template<typename T>
void makeNormal(T& c) {
    for(auto& val: c)
        if(!std::isnormal(val))
            val=0;
}
  
#endif // TASMET_UTILS_H
//////////////////////////////////////////////////////////////////////
