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

#include <QString>
#include <QSettings>
#include <QWidget>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QFileDialog>
#include <QStandardPaths>

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
    _filepath = "";
    set(_model);
}
void TaSMETMainWindow::loadModel() {
    TRACE(15,"loadModel");
    if(isDirty()) {
        int answer = saveFileFirstQuestion(this);
        if(answer == QMessageBox::Yes) saveModel();
        else if(answer == QMessageBox::Cancel) return;
    }

    QString title = tr("Open existing TaSMET model file");

    QString filepath = QFileDialog::getOpenFileName(this,
                                                    title,
                                                    start_file_location,
                                                    filetype);
    string filepath_s = filepath.toStdString();
    if(filepath.size()!=0) {
        try {
            TRACE(15,"Setting loaded model");
            _filepath = filepath_s;
            set(loadMessage<pb::Model>(filepath_s));
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
            saveMessage(*filepath,_model);
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

    _model = model;
    _system = *_model.mutable_system();
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
        dialog.set(segmap[id]);
    }

    int exitcode = dialog.exec();
    
    if(exitcode == QDialog::Accepted) {

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
    
    SolverDialog *d;
    try {
        d = new SolverDialog(this,_system,*_model.mutable_sparams());
    }
    catch(TaSMETError &e) {
        e.show_user("Solver failed to initialize");
        return;
    }
    d->exec();

    // Solution is put in system, system updated from solver
    // dialog. Therefore we are now probably dirty
    changed();

    delete d;
}
bool TaSMETMainWindow::isDirty() const {
    TRACE(15,"isDirty()");
    if(_filepath.size()==0) return true;

    try {
        pb::Model filemodel = loadMessage<pb::Model>(_filepath);
        bool dirty = !compareMessage<pb::Model>(filemodel,_model);
        VARTRACE(15,dirty);
        return dirty;
    }
    catch(TaSMETError& e) {
        TRACE(15,"Files could not be compared");
        return true;
    }
}

void TaSMETMainWindow::on_actionAbout_triggered(){
    
    AboutDialog(this).exec();
}

//////////////////////////////////////////////////////////////////////
