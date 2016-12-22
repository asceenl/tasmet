// mainwindow.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "mainwindow.h"
#include "tasmet_config.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QSettings>
#include <QWidget>
#include <QDoubleValidator>
#include <QIntValidator>
#include "gas.h"
#include "tasmet_tracer.h"
#include "add_duct_dialog.h"

const QString default_name = "Unnamed segment";

TaSMETMainWindow::TaSMETMainWindow():
    window(new Ui::MainWindow())
{

    window->setupUi(this);

    // Restore settings
    QSettings settings(company,appname);
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());        
        

    for(const SystemType& t: SystemType_vec){
        window->systemtype->addItem(SystemTypeToString(t));
    }
    for(int gastype = pb::GasType_MIN;gastype<=pb::GasType_MAX;gastype++){
        window->gastype->addItem(QString::fromStdString(GasType_Name((pb::GasType) gastype)));
    }
    for(const SegmentType& t: SegmentType_vec){
        window->segmenttype->addItem(SegmentTypeToString(t));
    }

    window->T0->setText(QString::number(constants::default_T0));
    window->T0->setValidator(new QDoubleValidator(constants::min_T0,
                                                  constants::max_T0,
                                                  constants::field_decimals));

    window->p0->setText(QString::number(constants::default_p0));
    window->p0->setValidator(new QDoubleValidator(constants::min_p0,
                                                  constants::max_p0,
                                                  constants::field_decimals));
    window->name->setText(default_name);

}
TaSMETMainWindow::~TaSMETMainWindow(){
    delete window;
}
void TaSMETMainWindow::closeEvent(QCloseEvent *event) {

    // Save window configuration to settings
    QSettings settings(company,appname);
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    // Forward close event to parent
    QMainWindow::closeEvent(event);
    
}
void TaSMETMainWindow::on_addsegment_clicked() {

    us id = window->segmentid_add->value();
    string name = window->name->text().toStdString();
    VARTRACE(15,name);
    bool edit = false;

    AddDuctDialog dialog(name,this);

    auto& ductmap = *_system.mutable_ducts();
    if(ductmap.find(id) != ductmap.end()) {
        dialog.set(ductmap[id]);
    }

    int exitcode = dialog.exec();
    
    if(exitcode == QDialog::Accepted) {

        VARTRACE(15,dialog.get().name());
        ductmap[id] = dialog.get();
        window->segmentid_add->setValue(id+1);

    }

    
}
void TaSMETMainWindow::on_segmentid_add_valueChanged(int id) {

    auto& ductmap = *_system.mutable_ducts();
    if(ductmap.find(id) != ductmap.end()) {
        window->name->setText(QString::fromStdString(ductmap[id].name()));
     }
}
void TaSMETMainWindow::on_name_textEdited() {

    auto& ductmap = *_system.mutable_ducts();
    int id = window->segmentid_add->value();
    
    if(ductmap.find(id) != ductmap.end()) {
        ductmap[id].set_name(window->name->text().toStdString());
        
    }
}
//////////////////////////////////////////////////////////////////////
