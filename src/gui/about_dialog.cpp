// about_dialog.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "about_dialog.h"
#include "ui_about_dialog.h"

AboutDialog::AboutDialog(QWidget* parent):
    QDialog(parent),
    _dialog(new Ui::about_dialog)
{
    _dialog->setupUi(this);
    
}
//////////////////////////////////////////////////////////////////////
