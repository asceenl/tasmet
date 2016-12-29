// about_dialog.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H
#include <QDialog>
namespace Ui{
    class about_dialog;
}
class AboutDialog:public QDialog{
    Ui::about_dialog* _dialog;
public:
    AboutDialog(QWidget* parent);
};

#endif // ABOUT_DIALOG_H
//////////////////////////////////////////////////////////////////////
