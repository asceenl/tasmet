// mainwindow.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include "tasmet_config.h"
#include "tasmet_tracer.h"
#include "tasmet_assert.h"
#include "tasmet_exception.h"
#include "add_duct_dialog.h"
#include "add_ductbc_dialog.h"
#include "tasystem.h"

#include "about_dialog.h"
#include "solver_dialog.h"
#include "message_tools.h"

#include <QString>
#include <QSettings>
#include <QWidget>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <QStandardPaths>
#include <QProcess>


const QString default_model_name = QString("Unsaved TaSMET Model") +
    constants::model_fileext;
const QString default_segment_name = "Unnamed segment";
const QString start_file_location = QStandardPaths::
    writableLocation(QStandardPaths::DocumentsLocation);
const QString filetype = QString("TaSMET Model files (*") + constants::model_fileext + ")";
const QString window_title_part = "TaSMET Model Builder - ";

namespace {

    int saveFileFirstQuestion(QWidget* parent) {

        return QMessageBox::question(parent,
                                     "Model has unsaved changes",
                                     "Would you like to save the changes?",
                                     QMessageBox::Yes |
                                     QMessageBox::No |
                                     QMessageBox::Cancel);
        
    }
}

TaSMETMainWindow::TaSMETMainWindow():
    _window(new Ui::MainWindow()),
    _model(pb::Model::default_instance()),
    _saved_model(_model),
    _system(*_model.mutable_system())
{

    if(!_window) throw TaSMETBadAlloc();
    _window->setupUi(this);

    // Restore settings
    QSettings settings(company,appname);
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());        

    for(const SystemType& t: SystemType_vec){
        _window->systemtype->addItem(SystemTypeToString(t));
    }
    for(int gastype = pb::GasType_MIN;gastype<=pb::GasType_MAX;gastype++){
        _window->gastype->addItem(QString::fromStdString(GasType_Name((pb::GasType) gastype)));
    }
    for(const SegmentType& t: SegmentType_vec){
        _window->segmenttype->addItem(SegmentTypeToString(t));
    }

    // TODO: change from omg to freq
    QValidator *omg_validator = new QDoubleValidator(constants::min_omg,
                                                     constants::max_omg,
                                                     constants::field_decimals);
    if(!omg_validator) throw TaSMETBadAlloc();
    omg_validator->setParent(this);
    _window->freq->setValidator(omg_validator);


    QValidator* T0_validator = new QDoubleValidator(constants::min_T0,
                                                    constants::max_T0,
                                                    constants::field_decimals);
    if(!T0_validator) throw TaSMETBadAlloc();
    T0_validator->setParent(this);
    _window->T0->setValidator(T0_validator);
    _window->T0->setText(QString::number(constants::default_T0));

    QValidator *p0_validator = new QDoubleValidator(constants::min_p0,
                                                    constants::max_p0,
                                                   constants::field_decimals); 
    if(!p0_validator) throw TaSMETBadAlloc();
    p0_validator->setParent(this);
    _window->p0->setValidator(p0_validator);
    _window->p0->setText(QString::number(constants::default_p0));

    _window->segmentname->setText(default_segment_name);

    _window->segmentid->setMaximum(constants::max_segs);

    _window->nf->setMaximum(constants::max_Nf);

    _init = false;

    changed();
    
}

TaSMETMainWindow::~TaSMETMainWindow(){
    delete _window;
}

void TaSMETMainWindow::newModel() {
    TRACE(15,"newModel");
    _model = pb::Model::default_instance();
    _saved_model = _model;
    _filepath = "";
    set(_model);
}
void TaSMETMainWindow::loadModel(const string* filepath_s) {
    TRACE(15,"loadModel");
    if(isDirty() && filepath_s == nullptr) {
        int answer = saveFileFirstQuestion(this);
        if(answer == QMessageBox::Yes) saveModel();
        else if(answer == QMessageBox::Cancel) return;
    }

    QString title = tr("Open existing TaSMET model file");

    if(filepath_s == nullptr) {

        QString filepath = QFileDialog::getOpenFileName(this,
                                                        title,
                                                        start_file_location,
                                                        filetype);
        if(filepath.size()!=0) {
            string filepath_s2 = filepath.toStdString();
            return loadModel(&filepath_s2);
        }
    }
    else {
        try {
            TRACE(15,"Setting loaded model");
            _filepath = *filepath_s;
            _model = loadMessage<pb::Model>(*filepath_s);
            _saved_model = _model;
            _system = *_model.mutable_system();
            set(_model);

        }
        catch(TaSMETError &e) {
            _filepath = "";
            e.show_user("Error opening model file");
        }
    }
}
void TaSMETMainWindow::saveModel(string* filepath) {

    TRACE(15,"saveModel");

    if(isDirty() || filepath!=nullptr) {

        // The user pressed save, but the model has never
        // been saved. Hence we 'save as'
        if(_filepath.size()==0 && filepath == nullptr) {
            return saveAsModel();
        }
        else if(filepath == nullptr) {
            filepath = &_filepath;
        }
        try {
            saveMessage<pb::Model>(*filepath,_model);
            _saved_model = _model;
            _filepath = *filepath;
            changed();
        }
        catch(TaSMETError &e) {

            e.show_user("Saving file failed");

        }
    }

}
void TaSMETMainWindow::saveAsModel() {
    TRACE(15,"saveModel");

    QString suggested_file;

    if(_filepath.size()==0) {
        suggested_file = QDir(start_file_location).filePath(default_model_name);
    }
    else {
        suggested_file.fromStdString(_filepath);
    }


    VARTRACE(15,filetype.toStdString());
    QString title = tr("Save model file");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    title,
                                                    suggested_file,
                                                    filetype);

    if(fileName.size()!=0) {

        // Add extension if necessary
        if(!fileName.contains(constants::model_fileext)) {
            fileName += constants::model_fileext;
        }

        string fn = fileName.toStdString();

        saveModel(&fn);
    }

}
void TaSMETMainWindow::changed() {
    TRACE(15,"changed()");
    if(_init) return;
    TRACE(15,"changed() continued");
    
    _system.set_nf(_window->nf->value());
    _system.set_freq(_window->freq->text().toDouble());

    _system.set_gastype((pb::GasType) _window->gastype->currentIndex());
    _system.set_t0(_window->T0->text().toDouble());
    _system.set_p0(_window->p0->text().toDouble());
    
    _system.set_systemtype((pb::SystemType) _window->systemtype->currentIndex());

    QString windowtitle = window_title_part;
    if(isDirty()) windowtitle+= '*';
    if(_filepath.size()!=0) {
        windowtitle += QString::fromStdString(_filepath);
    }
    else {
        windowtitle += default_model_name;
    }

    setWindowTitle(windowtitle);
    _window->segoverview->setPlainText(QString::fromStdString(_system.DebugString()));

    // Update stuff based on the segments
    int segid = _window->segmentid->value();

    bool is_segment = false;

    auto& ductmap = *_system.mutable_ducts();
    auto& ductbcmap = *_system.mutable_ductbcs();

    if(ductmap.find(segid) != ductmap.end()) {
        _window->segmentname->setText(QString::fromStdString(ductmap[segid].name()));
        _window->segmenttype->setCurrentIndex((int) Duct);
        is_segment = true;
    }
    if(ductbcmap.find(segid) != ductbcmap.end()) {
        _window->segmentname->setText(QString::fromStdString(ductbcmap[segid].name()));
        _window->segmenttype->setCurrentIndex((int) DuctBc);
        is_segment = true;        
    }

    _window->removesegment->setEnabled(is_segment);
    _window->segmenttype->setEnabled(!is_segment);

    QString backlog = _window->backlog->toPlainText();
    *_model.mutable_backlog() = backlog.toStdString();

}
void TaSMETMainWindow::set(const pb::Model& model) {
    TRACE(15,"set()");
    _init = true;

    const pb::System& sys = model.system();
    _window->nf->setValue(sys.nf());
    _window->freq->setText(QString::number(sys.freq()));
    _window->p0->setText(QString::number(sys.p0()));
    _window->T0->setText(QString::number(sys.t0()));
    _window->systemtype->setCurrentIndex((int) sys.systemtype());
    _window->gastype->setCurrentIndex((int) sys.gastype());

    _window->backlog->setPlainText(QString::fromStdString(model.backlog()));

    _init = false;

    changed();

}
void TaSMETMainWindow::closeEvent(QCloseEvent *event) {

    if(isDirty()) {
        
        int answer = saveFileFirstQuestion(this);
        if(answer == QMessageBox::Yes){
            saveModel();
            // If we are still dirty after a save (user probably
            // cancelled the save action, return to the GUI
            if(isDirty()) {
                TRACE(15,"still dirty");
                event->ignore();
                return;
            }
        }
        else if(answer == QMessageBox::Cancel) {
            TRACE(15,"CloseEvent cancelled");
            event->ignore();
            return;
        }
    } 

    // If we are here, the model is not dirty anymore. We can savely
    // close the application.
    
    // Save window configuration to settings
    QSettings settings(company,appname);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    event->accept();
    
}
// Different segments all have different type. Therefore this part of
// the code is templatized. In this function, the dialog is
// created. If the segment already existed, the `set' function is
// called, with the found segment in the list. Finally the exitcode of
// the dialog is returned
template<typename segmenttype,typename dialogtype>
int add_edit_segment(QWidget* parent,
                     const string& name,
                     google::protobuf::Map<google::protobuf::uint32,segmenttype> & segmap,
                     const us id) {
    
    dialogtype dialog(name,parent);
    
    if(segmap.find(id) != segmap.end()) {
        TRACE(16,"Found the segment in the list");
        dialog.set(segmap[id]);
    }

    int exitcode = dialog.exec();
    
    if(exitcode == QDialog::Accepted) {
        TRACE(16,"Updating existing segment");
        segmap[id] = dialog.get();
        
    }
    return exitcode;
}
                 

void TaSMETMainWindow::on_addsegment_clicked() {
    TRACE(15,"on_addsegment_clicked");
    us id = _window->segmentid->value();
    string name = _window->segmentname->text().toStdString();
    VARTRACE(15,name);

    SegmentType segtype = (SegmentType) _window->segmenttype->currentIndex();

    int exitcode=QDialog::Rejected;

   switch (segtype) {
    case Duct: {
        exitcode = add_edit_segment<pb::Duct,AddDuctDialog>
            (this,
             name,
             *_system.mutable_ducts(),
             id);
        break;
    }
    case DuctBc: {
        exitcode = add_edit_segment<pb::DuctBc,AddDuctBcDialog>
            (this,
             name,
             *_system.mutable_ductbcs(),
             id);
        break;
    }
        
    default:
        tasmet_assert(false,"Not implemented");
    }

   if(exitcode == QDialog::Accepted) {
       changed();
   }

}

void TaSMETMainWindow::on_removesegment_clicked() {
    TRACE(15,"on_remove");
    QString question = "Are you you wish to delete segment ";
    int segid = _window->segmentid->value();

    question+= QString::number(segid);
    question+= "?";
    int answ = QMessageBox::question(this,
                                     "Deleting segment",
                                     question,
                                     QMessageBox::Yes | QMessageBox::No);
    if(answ == QMessageBox::Yes) {
        _system.mutable_ducts()->erase(segid);
        _system.mutable_ductbcs()->erase(segid);
        changed();
    }
}

void TaSMETMainWindow::on_segmentname_textChanged() {
    if(_init) return;
    auto& ductmap = *_system.mutable_ducts();
    auto& ductbcmap = *_system.mutable_ductbcs();

    int id = _window->segmentid->value();

    if(ductmap.find(id) != ductmap.end()) {
        ductmap[id].set_name(_window->segmentname->text().toStdString());
    }
    if(ductbcmap.find(id) != ductbcmap.end()) {
        ductbcmap[id].set_name(_window->segmentname->text().toStdString());
    }
    changed();
}

void TaSMETMainWindow::on_actionSolve_triggered() {
    TRACE(15,"actionSolve()");
    
    std::unique_ptr<SolverDialog> d;

    std::unique_ptr<TaSystem> sys;
    try {
        sys = std::unique_ptr<TaSystem>(new TaSystem(_model.system()));
        
        if(_model.solution_size()>0) {
            vd solution(_model.solution_size());
            for(int i=0;i<_model.solution_size();i++) {
                solution(i) = _model.solution(i);
            }

            // Throws in case the solution size does not match.
            try {
                sys->updateSolution(solution);
            }
            catch(...) {}
        }
        d = std::unique_ptr<SolverDialog>(new SolverDialog(this,*sys.get(),*_model.mutable_sparams()));
    }

    catch(TaSMETError &e) {
        e.show_user("Solver failed to initialize");
        return;
    }

    if(d->exec()) {

        // On succes, we copy the solution to the model
        const vd& sol = d->getSolution();
        
        // Clear old solution
        _model.clear_solution();
        for(us i=0;i<sol.size();i++) {
            _model.add_solution(sol(i));
        }

    }

    // Solution is put in system, system updated from solver
    // dialog. Therefore we are now probably dirty
    changed();

}
void TaSMETMainWindow::on_actionPostprocess_model_triggered() {

    try {
        
        TaSystem sys(_model.system());
        if(_model.solution_size() == 0)
            throw TaSMETError("No solution found");
        vd sol(_model.solution_size());
        
        for(us i=0;i<sol.size();i++) {
            sol(i) = _model.solution(i);
        }
        sys.updateSolution(sol);

        if(_filepath.size() == 0)
            throw TaSMETError("Model has not yet been saved");
        sys.exportHDF5(_filepath + ".h5");

        QString command = "python";
        QString appfilepath = QCoreApplication::applicationDirPath();
        QString postprocessor_program = appfilepath +
            QDir::separator() + "tasmet_postprocessor.py";
        QString filepath_post = QString::fromStdString(_filepath) + ".h5";

        QStringList params;        
        params << postprocessor_program;
        params << filepath_post;

        // Start the process, which is destroyed whenever the program
        // is killed
        QProcess *p = new QProcess(this);
        // connect(p,&QProcess::finished,p,&QProcess::deleteLater);
        p->startDetached(command,params);
    }
    catch(TaSMETError &e) {
        e.show_user("Postprocessing failed");
    }

}
bool TaSMETMainWindow::isDirty() const {
    TRACE(15,"isDirty()");
    return !compareMessage(_model,_saved_model);
}

void TaSMETMainWindow::on_actionAbout_triggered(){
    
    AboutDialog(this).exec();
}
void TaSMETMainWindow::on_actionReinitialize_solution_triggered() {
    
    _model.clear_solution();
    changed();
}

//////////////////////////////////////////////////////////////////////
