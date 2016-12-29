// add_ductbc_dialog.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////

#include "add_ductbc_dialog.h"
#include "ui_add_ductbc_dialog.h"
#include "tasmet_tracer.h"
#include "tasmet_assert.h"

AddDuctBcDialog::AddDuctBcDialog(const std::string& name,QWidget* parent):
    QDialog(parent),
    _dialog(new Ui::add_ductbc_dialog),
    _ductbc(pb::DuctBc::default_instance())
{
    TRACE(15,"AddDuctDialog");
    _dialog->setupUi(this);

    _ductbc.set_name(name);

    QString title = "Add/edit duct boundary condition segment '";
    title += QString::fromStdString(name);
    title +="'";
    this->setWindowTitle(title);

    for(int ductbctype = pb::DuctBcType_MIN;
        ductbctype<=pb::DuctBcType_MAX;
        ductbctype++){
        _dialog->type->addItem(QString::fromStdString(DuctBcType_Name((pb::DuctBcType) ductbctype)));
    }
    _dialog->type->setCurrentIndex(0);

    for(int ductside = pb::DuctSide_MIN;
        ductside<=pb::DuctSide_MAX;
        ductside++){
        _dialog->side->addItem(QString::fromStdString(DuctSide_Name((pb::DuctSide) ductside)));
    }
    _dialog->side->setCurrentIndex(0);

    _dialog->pressure->setText("p0 + 1.0*sin(2*pi*freq*t)");
    _dialog->temperature->setText("T0");

    _init = false;
    changed();

}
void AddDuctBcDialog::changed() {
    if(_init) return;
    pb::DuctBcType type = (pb::DuctBcType) _dialog->type->currentIndex();
    bool isentropic =  _dialog->isentropic->isChecked();
    VARTRACE(15,type);
    switch (type) {
    case pb::PressureBc: 
        _dialog->isentropic->setEnabled(true);
        _dialog->temperature->setEnabled(!isentropic);
        _dialog->pressure->setEnabled(true);
        break;
    case pb::AdiabaticWall:
        _dialog->isentropic->setEnabled(false);
        _dialog->temperature->setEnabled(false);
        _dialog->pressure->setEnabled(false);
        break;
    case pb::IsoTWall:
        _dialog->isentropic->setEnabled(false);
        _dialog->temperature->setEnabled(true);
        _dialog->pressure->setEnabled(false);
        break;
    default:
        tasmet_assert(false,"Not implemented");
        break;
    }
    _ductbc.set_type(type);
    _ductbc.set_side((pb::DuctSide) _dialog->side->currentIndex());
    _ductbc.set_duct_id(_dialog->duct_id->value());
    _ductbc.set_pressure(_dialog->pressure->text().toStdString());
    _ductbc.set_temperature(_dialog->temperature->text().toStdString());
    _ductbc.set_isentropic(isentropic);
    
}
void AddDuctBcDialog::set(const pb::DuctBc& ductbc) {

    _dialog->type->setCurrentIndex((int) ductbc.type());
    _dialog->side->setCurrentIndex((int) ductbc.side());
    _dialog->duct_id->setValue(ductbc.duct_id());
    _dialog->pressure->setText(QString::fromStdString(ductbc.pressure()));
    _dialog->temperature->setText(QString::fromStdString(ductbc.temperature()));
    _dialog->isentropic->setChecked(ductbc.isentropic());
    _ductbc = ductbc;
}

AddDuctBcDialog::~AddDuctBcDialog(){
    delete _dialog;

}
void AddDuctBcDialog::reject() {
    QDialog::reject();
}
void AddDuctBcDialog::accept() {
    QDialog::accept();
}
//////////////////////////////////////////////////////////////////////
