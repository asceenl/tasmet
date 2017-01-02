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
#include <QDialog>
#include <QThread>

#include "solver.pb.h"

namespace pb {
    class System;
}
namespace Ui {
    class solver_dialog;
}
class QCustomPlot;
class QCPGraph;
class SolverWorker;
class SolverProgress;

class SolverDialog: public QDialog {
    Q_OBJECT

    pb::System& _sys;           // Reference to system

    Ui::solver_dialog* _dialog;

    bool _init = true;

    QCustomPlot* _plot;
    QCPGraph *_funer,*_reler,*_funtol,*_reltol;

    SolverWorker* _solver_worker = nullptr;

public:

    SolverDialog(QWidget* parent,
                 pb::System& sys);
    
    ~SolverDialog();

    void set(const pb::SolverParams&);
public slots:
    void solver_progress(const SolverProgress&);
private slots:
    // Connected to the signal from the solver thread. Reactivates the
    // buttons
    void solver_stopped(bool converged);

    void on_solve_clicked();
    void on_singleiteration_clicked();
    void on_stop_clicked();

    void on_funtol_textChanged() { changed();}
    void on_reltol_textChanged() { changed();}
    void on_solvertype_currentIndexChanged(int) { changed();}
private:
    // Called whenever the user changes input values
    void changed();
    void setEnabled(bool);
};
    


#endif // SOLVER_DIALOG_H
//////////////////////////////////////////////////////////////////////
