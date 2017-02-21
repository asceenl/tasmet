// add_ductbc_dialog.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ADD_DUCTBC_DIALOG_H
#define ADD_DUCTBC_DIALOG_H
#include "ductbc.pb.h"
#include <QDialog>
namespace Ui{
    class add_ductbc_dialog;
}
class AddDuctBcDialog: public QDialog {
    Q_OBJECT
    Ui::add_ductbc_dialog* _dialog;
    pb::DuctBc _ductbc;
    bool _init = true;
public:
    AddDuctBcDialog(const std::string& name,QWidget* parent = nullptr);
    ~AddDuctBcDialog();

    void set(const pb::DuctBc&);
    const pb::DuctBc& get() const {return _ductbc; }

private:
    void changed();

private slots:
    void on_type_currentIndexChanged(int) {changed();}
    void on_duct_id_valueChanged(int) { changed();}
    void on_side_currentIndexChanged(int) {changed();}    
    void on_isentropic_stateChanged(int) {changed();}
    void on_pressure_textChanged() {changed();}
    void on_temperature_textChanged() {changed();}
    void accept();
    void reject();

};

#endif // ADD_DUCTBC_DIALOG_H
//////////////////////////////////////////////////////////////////////
