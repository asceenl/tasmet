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
#include "protobuf/model.pb.h"
#include "tasmet_types.h"

namespace Ui{
    class MainWindow;
}

class TaSMETMainWindow: public QMainWindow {
    // For Qt
    Q_OBJECT

    Ui::MainWindow *_window;

    // In-memory model
    pb::Model _model;

    pb::Model _saved_model;     /**< Last saved state of the model, to
                                   compare with to check whether we
                                   are dirty yes or no */

    pb::System& _system;        /**< In constructor set to the
                                   _model.mutable_system() */

    // Where the file is stored
    string _filepath = "";

    bool _init = true;          /**<  */

public:
    TaSMETMainWindow();
    ~TaSMETMainWindow();
private:
    void newModel();
public:
    /** 
     * Load a model from a file. Called by File->Open. If no filepath
     * is given, a dialog is opened to specify the file. This function
     * is public such that it can be called for main.cpp.
     *
     * @param filepath the filepath of the file to load
     */
    void loadModel(const string* filepath=nullptr);
private:
    /** 
     * Save the model to a file. If no filepath is given, a dialog is
     * opened to ask the user for a filepath.
     *
     * @param filepath 
     */
    void saveModel(string* filepath=nullptr);
    /** 
     * Save the model with a different filename.
     */
    void saveAsModel();

    /** 
     * When the user interacts, we call this function to update the
     * internal state and set the widget status accordingly.
     */
    void changed();
    /** 
     * Change the widget states according to the model 
     *
     * @param model The model to set
     */
    void set(const pb::Model& model);

    /** 
     * Check whether the filepath contents agree with the system in
     * memory.
     *
     * @return true when the model in memory does not agree with the
     * file contents.
     */
    bool isDirty() const;

private slots:
    void closeEvent(QCloseEvent *event);
    void on_addsegment_clicked();
    void on_removesegment_clicked();
    void on_segmentid_valueChanged(int i) {changed();}
    void on_segmentname_textChanged();

    // Couple slots to functions
    void on_actionNew_triggered() { newModel();}
    void on_actionOpen_triggered() { loadModel();}
    void on_actionSave_triggered() { saveModel();}
    void on_actionSaveAs_triggered() { saveAsModel();}
    void on_actionExit_triggered() { closeEvent(NULL);}    
    void on_actionAbout_triggered(); // Show about dialog
    void on_actionSolve_triggered(); // Solve the system

    void on_actionReinitialize_solution_triggered();
    void on_actionPostprocess_model_triggered();

    void on_nf_valueChanged(int) {changed();}
    void on_freq_textEdited() {changed();}
    void on_gastype_currentIndexChanged(int) {changed();}
    void on_T0_textEdited() {changed();}
    void on_p0_textEdited() {changed();}
    void on_systemtype_currentIndexChanged(int) {changed();}

    void on_backlog_textChanged() {changed();}

};



#endif // MAINWINDOW_H
//////////////////////////////////////////////////////////////////////





