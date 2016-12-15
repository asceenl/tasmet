// solver_dialog.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SOLVER_DIALOG_H
#define SOLVER_DIALOG_H


// Put here, temporarily
    window->funtol->setText(QString::number(constants::default_funtol));
    window->funtol->setValidator(new QDoubleValidator(constants::min_funtol,
                                                      constants::max_funtol,
                                                      constants::field_decimals));
    window->reltol->setText(QString::number(constants::default_funtol));
    window->reltol->setValidator(new QDoubleValidator(constants::min_funtol,
                                                      constants::max_funtol,
                                                      constants::field_decimals));

    window->maxiter->setText(QString::number(constants::default_maxiter));
    window->maxiter->setValidator(new QIntValidator(constants::min_maxiter,
                                                      constants::max_maxiter));

    for(const SolverType& t: SolverType_vec){
        window->solvertype->addItem(SolverTypeToString(t));
    }


#endif // SOLVER_DIALOG_H
//////////////////////////////////////////////////////////////////////
