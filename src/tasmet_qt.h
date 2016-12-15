// tasmet_qt.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TASMET_QT_H
#define TASMET_QT_H
#include "tasmet_types.h"

template<typename T>
inline QVector<T> from_arma(const arma::Col<T>& arma_vec){

    QVector<T> qvec(arma_vec.size());
    for(us i=0;i<arma_vec.size();i++){
        qvec[i] = arma_vec(i);
    }
    return qvec;
}


#endif // TASMET_QT_H
//////////////////////////////////////////////////////////////////////
