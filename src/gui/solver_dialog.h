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
#include "tasmet_types.h"
#include "solver.pb.h"

namespace pb {
    class System;
    class SolverParams;
}
namespace Ui {
    class solver_dialog;
}
class GradientNonlinearSystem;
class QCustomPlot;
class QCPGraph;
class SolverWorker;
class SolverProgress;

class SolverDialog: public QDialog {
    Q_OBJECT

    pb::SolverParams& _sparams;

    Ui::solver_dialog* _dialog;

    bool _init = true;

    QCustomPlot* _plot;
    QCPGraph *_funer,*_reler,*_funtol,*_reltol;

    SolverWorker* _solver_worker = nullptr;

    const GradientNonlinearSystem& _sys;

    /// Place where the final solution will be stored
    vd _solution;

public:

    SolverDialog(QWidget* parent,
                 const GradientNonlinearSystem& sys,
                 pb::SolverParams& sparams);
    
    ~SolverDialog();

    const vd& getSolution() const { return _solution;};

public slots:
    void solver_progress(const SolverProgress&);
private slots:
    // Connected to the signal from the solver thread. Reactivates the
    // buttons
    void solver_stopped(bool converged);

    void on_solve_clicked();
    void on_stop_clicked();

    void on_funtol_textChanged() { changed();}
    void on_reltol_textChanged() { changed();}
    void on_solvertype_currentIndexChanged(int) { changed();}

    void on_buttons_accepted() {accept();}
    void on_buttons_rejected() {reject();}

private:

    // Called whenever the user changes input values
    void changed();
    void setEnabled(bool);
    void set(const pb::SolverParams&);


};
    


#endif // SOLVER_DIALOG_H
//////////////////////////////////////////////////////////////////////
