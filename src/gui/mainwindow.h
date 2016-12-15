// mainwindow.h
//
// Author: J.A. de Jong 
//
// Description:
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "tasmet_config.h"
#include <QMainWindow>

namespace Ui{
    class MainWindow;
}

class TaSMETMainWindow: public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *window;
public:
    TaSMETMainWindow();
    ~TaSMETMainWindow();
private slots:
    void closeEvent(QCloseEvent *event);
    void on_addsegment_clicked();
};



#endif // MAINWINDOW_H
//////////////////////////////////////////////////////////////////////





