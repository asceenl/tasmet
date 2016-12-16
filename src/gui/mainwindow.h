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
#include "protobuf/system.pb.h"

namespace Ui{
    class MainWindow;
}

class TaSMETMainWindow: public QMainWindow {
    Q_OBJECT
    Ui::MainWindow *window;
    pb::System _system;
public:
    TaSMETMainWindow();
    ~TaSMETMainWindow();
private slots:
    void closeEvent(QCloseEvent *event);
    void on_addsegment_clicked();
    void on_segmentid_add_valueChanged(int i);
    void on_name_textEdited();
};



#endif // MAINWINDOW_H
//////////////////////////////////////////////////////////////////////





