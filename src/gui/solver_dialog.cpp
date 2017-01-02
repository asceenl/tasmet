// solver_dialog.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "solver_dialog.h"
#include "ui_solver_dialog.h"
#include "system.pb.h"
#include "tasmet_exception.h"
#include "tasmet_constants.h"
#include "tasmet_tracer.h"
#include "solver.pb.h"
#include <QSharedPointer>
#include <QColor>
#include "solver_worker.h"
#include <algorithm>
#include <qcustomplot.h>

SolverDialog::SolverDialog(QWidget* parent,
                           pb::System& sys):
    QDialog(parent),
    _sys(sys),
    _dialog(new Ui::solver_dialog())
{
    
    TRACE(15,"SolverDialog");
    if(_dialog==nullptr) throw TaSMETBadAlloc();

    _dialog->setupUi(this);

    _dialog->funtol->setValidator(new QDoubleValidator(constants::min_funtol,
                                                      constants::max_funtol,
                                                      constants::field_decimals));

    _dialog->reltol->setValidator(new QDoubleValidator(constants::min_funtol,
                                                      constants::max_funtol,
                                                      constants::field_decimals));

    // _dialog->maxiter->setValidator(new QIntValidator(constants::min_maxiter,
    //                                                 constants::max_maxiter));

    for(int solvertype = pb::SolverType_MIN;solvertype<=pb::SolverType_MAX;solvertype++){
        _dialog->solvertype->addItem(QString::fromStdString(SolverType_Name((pb::SolverType) solvertype)));
    }

    // Set the residual plot settings right

    _plot = _dialog->progress_plot;

    _funer = _plot->addGraph();
    _funer->setName("Residual");
    QPen funer_pen(Qt::blue);
    _funer->setPen(QPen(Qt::blue));

    _reler = _plot->addGraph();
    _reler->setName("Solution error");
    QPen reler_pen(Qt::red);
    _reler->setPen(reler_pen);

    _funtol = _plot->addGraph();
    _funtol->setName("Residual tolerance");
    QPen funtol_pen(Qt::blue);
    funtol_pen.setStyle(Qt::DashLine);
    _funtol->setPen(funtol_pen);

    _reltol = _plot->addGraph();
    _reltol->setName("Solution tolerance");
    QPen reltol_pen(Qt::red);
    reltol_pen.setStyle(Qt::DashLine);
    _reltol->setPen(reltol_pen);
    
    _plot->xAxis->setLabel("Iteration [-]");
    _plot->yAxis->setLabel("Residual [-]");
    
    _plot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    QSharedPointer<QCPAxisTicker> ticker(new QCPAxisTickerLog());
    _plot->yAxis->setTicker(ticker);


    // Toggle legend
    _plot->legend->setVisible(true);

    _plot->replot();
    set(_sys.solverparams());

    setEnabled(true);

    _init = false;

}
SolverDialog::~SolverDialog() {
    if(_solver_worker) {
        // deleteLater is called on this one, so no delete here
        _solver_worker->solver_stop();
    }
}
void SolverDialog::set(const pb::SolverParams& sol) {
    TRACE(15,"set");
    int solvertype= (int) sol.solvertype();
    _dialog->solvertype->setCurrentIndex(solvertype);

    d funtol = sol.funtol();
    _dialog->funtol->setText(QString::number(funtol));

    d reltol = sol.reltol();
    _dialog->reltol->setText(QString::number(reltol));
    
}
void SolverDialog::changed() {
    TRACE(15,"changed");
    if(_init) return;
    _sys.mutable_solverparams()->set_funtol(_dialog->funtol->text().toDouble());
    _sys.mutable_solverparams()->set_reltol(_dialog->reltol->text().toDouble());
    _sys.mutable_solverparams()->set_solvertype((pb::SolverType) _dialog->solvertype->currentIndex());
}
void SolverDialog::solver_progress(const SolverProgress& progress){

    TRACE(15,"SolverDialog::solver_progress()");

    // VARTRACE(15,progress.fun_err);
    // VARTRACE(15,progress.iteration);
    d funtol = _sys.solverparams().funtol();
    d reltol = _sys.solverparams().reltol();

    _funer->addData(progress.iteration,progress.fun_err);
    _reler->addData(progress.iteration,progress.rel_err);    

    _funtol->addData(progress.iteration,funtol);
    _reltol->addData(progress.iteration,reltol);

    _plot->xAxis->setRange(0,progress.iteration);
    _plot->yAxis->setRange(std::min(funtol,reltol)/10,1e0);    

    _plot->replot();

}
void SolverDialog::on_stop_clicked(){
    tasmet_assert(_solver_worker!=nullptr,"No _solver_worker");
    _solver_worker->solver_stop();
}
void SolverDialog::on_solve_clicked() {
    TRACE(15,"on_solve_clicked");

    // Disable buttons
    setEnabled(false);

    // Clear figure data
    QVector<double> empty;
    _funer->setData(empty,empty);
    _reler->setData(empty,empty);
    _funtol->setData(empty,empty);
    _reltol->setData(empty,empty);


    assert(!_solver_worker);

    qRegisterMetaType<SolverProgress>();

    _solver_worker = new SolverWorker(_sys);
    QThread* thread = new QThread;

    _solver_worker->moveToThread(thread);
    

    connect(thread, &QThread::started,
            _solver_worker, &SolverWorker::solver_start);

    connect(_solver_worker, &SolverWorker::solver_stopped,
            thread, &QThread::quit);

    connect(_solver_worker, &SolverWorker::solver_stopped,
            this, &SolverDialog::solver_stopped);

    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);

    connect(thread, &QThread::finished,
            _solver_worker, &SolverWorker::deleteLater);

    connect(_solver_worker, &SolverWorker::progress,
            this, &SolverDialog::solver_progress);
    
    thread->start();


}
void SolverDialog::on_singleiteration_clicked() {
    
}
void SolverDialog::setEnabled(bool enabled){
    _dialog->solve->setEnabled(enabled);
    _dialog->singleiteration->setEnabled(enabled);
    _dialog->stop->setEnabled(!enabled);

    _dialog->solvertype->setEnabled(enabled);
    _dialog->reltol->setEnabled(enabled);
    _dialog->funtol->setEnabled(enabled);

}
void SolverDialog::solver_stopped(bool converged) {
    setEnabled(true);
    // stop the solver and delete it
    if(_solver_worker!=nullptr) {
        _solver_worker->solver_stop();
        _solver_worker = nullptr;
    }

    if(converged) {
        QMessageBox::information(this,
                                 "Solution converged",
                                 "Solution found within required tolerance");
                                 
    }
    else {
        QMessageBox::information(this,
                                 "Solver failed",
                                 "Failed to find solution");

    }
}

//////////////////////////////////////////////////////////////////////
