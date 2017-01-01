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
#include "solver_worker.h"

SolverDialog::SolverDialog(QWidget* parent,
                           pb::System& sys):
    QDialog(parent),
    _sys(sys),
    _sparams(sys.solverparams()),
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

    _plot = _dialog->progress_plot;
    _plot->xAxis->setLabel("Iteration [-]");
    _plot->yAxis->setLabel("Residual [-]");
    
    _plot->yAxis->setScaleType(QCPAxis::stLogarithmic);

    QSharedPointer<QCPAxisTicker> ticker(new QCPAxisTickerLog());
    _plot->yAxis->setTicker(ticker);
    
    _plot->replot();
    set(_sys.solverparams());

}
SolverDialog::~SolverDialog() {
    if(_solver_worker)
        _solver_worker->solver_stop();
}
void SolverDialog::set(const pb::SolverParams& sol) {
    TRACE(15,"set");

    _dialog->solvertype->setCurrentIndex((int) sol.solvertype());
    _dialog->funtol->setText(QString::number(sol.funtol()));
    _dialog->reltol->setText(QString::number(sol.reltol()));
    
}
void SolverDialog::changed() {

}
void SolverDialog::on_stop_clicked(){
    tasmet_assert(_solver_worker!=nullptr,"No _solver_worker");
    _solver_worker->solver_stop();
}
void SolverDialog::on_solve_clicked() {
    TRACE(15,"on_solve_clicked");
    _dialog->solve->setEnabled(false);
    _dialog->singleiteration->setEnabled(false);
    _dialog->stop->setEnabled(true);

    assert(!_solver_worker);

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

    
    thread->start();


}
void SolverDialog::on_singleiteration_clicked() {
    _dialog->solve->setEnabled(false);
    _dialog->singleiteration->setEnabled(false);
    _dialog->stop->setEnabled(true);


}

void SolverDialog::solver_stopped() {
    _dialog->solve->setEnabled(true);
    _dialog->singleiteration->setEnabled(true);
    _dialog->stop->setEnabled(false);

    // stop the solver and delete it
    if(_solver_worker!=nullptr) {
        _solver_worker->solver_stop();
        _solver_worker = nullptr;
    }
}

//////////////////////////////////////////////////////////////////////
