// add_duct_dialog.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ADD_DUCT_DIALOG_H
#define ADD_DUCT_DIALOG_H
#include <QDialog>
#include <memory>
#include "duct.pb.h"
#include "tasmet_types.h"

namespace Ui{
    class add_duct_dialog;
}
class QCustomPlot;
class Grid;

class AddDuctDialog: public QDialog {
    
    Q_OBJECT
    Ui::add_duct_dialog *_dialog;
    QCustomPlot* _plot;
    pb::Duct _duct;
    bool _blocked = false;
public:
    AddDuctDialog(const std::string& name,QWidget* parent = nullptr);
    ~AddDuctDialog();

    void set(const pb::Duct&);
    const pb::Duct& get() const {return _duct; }

private slots:
    void accept();
    void reject();

    void on_L_textEdited() {changed();}
    void on_S_textEdited() {changed();}
    void on_phi_textEdited() {changed();}
    void on_rh_textEdited() {changed();}
    void on_cshape_currentIndexChanged(int) {changed();}
    void on_solidtype_currentIndexChanged(int) {changed();}    

    void on_gridtype_currentIndexChanged(int) {changed();}
    void on_ngp_textEdited() {changed();}
    void on_dxb_textEdited() {changed();}
    void on_dxmid_textEdited() {changed();}
    
    void on_htmodel_currentIndexChanged(int) {changed();}
    void on_stempfunc_textEdited() {changed();}
    void on_stempmodel_currentIndexChanged(int) {changed();}

    void on_previewshow_currentIndexChanged(int) {changed();}    

private:
    // Called whenever the user changes a field
    void changed();
    

};


#endif // ADD_DUCT_DIALOG_H
//////////////////////////////////////////////////////////////////////
