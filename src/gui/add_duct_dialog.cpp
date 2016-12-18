// add_duct_dialog.cpp
//
// last-edit-by: J.A. de Jong 
// 
// Description:
//
//////////////////////////////////////////////////////////////////////
#include "ui_add_duct_dialog.h"
#include "add_duct_dialog.h"

#include <QSignalBlocker>
#include <qcustomplot.h>
#include "tasmet_constants.h"
#include "tasmet_enum.h"
#include "tasmet_tracer.h"
#include "tasmet_assert.h"
#include "tasmet_exception.h"
#include "duct/grid.h"
#include <QVector>
#include "tasmet_qt.h"
#include "geom.h"

DECLARE_ENUM(PreviewShow,CSArea,Porosity,HydraulicRadius,SolidTemperatureFunction)



AddDuctDialog::AddDuctDialog(const std::string& name,QWidget* parent):
    QDialog(parent),
    _dialog(new Ui::add_duct_dialog),
    _duct(pb::Duct::default_instance())
{
    TRACE(15,"AddDuctDialog");
    _blocked = true;
    _dialog->setupUi(this);

    _duct.set_name(name);

    QString title = "Add/edit duct segment '";
    title += QString::fromStdString(name);
    title +="'";
    this->setWindowTitle(title);


    _plot = _dialog->plot;

    // Put validators on pure numeric fields
    _dialog->L->setValidator(new QDoubleValidator(constants::min_L,
                                                  constants::max_L,
                                                  constants::field_decimals));
    _dialog->ngp->setValidator(new QIntValidator(constants::min_ngp,
                                                 constants::max_ngp
                                                 ));
    _dialog->dxb->setValidator(new QDoubleValidator(1e-16,
                                                    1e6,
                                                    constants::field_decimals));
    
    _dialog->dxmid->setValidator(new QDoubleValidator(1e-16,
                                                      1e6,
                                                      constants::field_decimals));
    
    
    for(const PreviewShow& t: PreviewShow_vec){
        _dialog->previewshow->addItem(PreviewShowToString(t));
    }
    
    for(int cshape = pb::Cshape_MIN;cshape<=pb::Cshape_MAX;cshape++){
        _dialog->cshape->addItem(QString::fromStdString(Cshape_Name((pb::Cshape) cshape)));
    }
    _dialog->cshape->setCurrentIndex(0);

    for(int SolidType = pb::SolidType_MIN;SolidType<=pb::SolidType_MAX;SolidType++){
        _dialog->solidtype->addItem(QString::fromStdString(SolidType_Name((pb::SolidType) SolidType)));
    }
    _dialog->solidtype->setCurrentIndex(0);

    for(int HeatTransferModel = pb::HeatTransferModel_MIN;HeatTransferModel<=pb::HeatTransferModel_MAX;HeatTransferModel++){
        _dialog->htmodel->addItem(QString::fromStdString(HeatTransferModel_Name((pb::HeatTransferModel) HeatTransferModel)));
    }
    _dialog->htmodel->setCurrentIndex(0);
    
    for(int DragModel = pb::DragModel_MIN;DragModel<=pb::DragModel_MAX;DragModel++){
        _dialog->dragmodel->addItem(QString::fromStdString(DragModel_Name((pb::DragModel) DragModel)));
    }
    _dialog->dragmodel->setCurrentIndex(0);
    
    for(int SolidTemperatureModel = pb::SolidTemperatureModel_MIN;SolidTemperatureModel<=pb::SolidTemperatureModel_MAX;SolidTemperatureModel++){
        _dialog->stempmodel->addItem(QString::fromStdString(SolidTemperatureModel_Name((pb::SolidTemperatureModel) SolidTemperatureModel)));
    }
    _dialog->stempmodel->setCurrentIndex(0);
    for(int gridtype = pb::GridType_MIN;gridtype<=pb::GridType_MAX;gridtype++){
        _dialog->gridtype->addItem(QString::fromStdString(GridType_Name((pb::GridType) gridtype)));
    }
    _dialog->gridtype->setCurrentIndex(0);

    // create a single graph in the QCP
    QCPScatterStyle scatter;
    scatter.setShape(QCPScatterStyle::ssDiamond);
    _plot->addGraph();
    _plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    _plot->graph(0)->setScatterStyle(scatter);    
    
    _duct.set_length(constants::default_L);
    _duct.set_dxb(0.01);
    _duct.set_dxmid(0.1);
    _duct.set_gridtype(pb::Linear);
    _duct.set_ngp(constants::default_ngp);
    _duct.set_area("1.0+0*x/L");
    _duct.set_phi("1.0+0*x/L");
    _duct.set_rh("sqrt(1/pi)");
    _duct.set_cshape(pb::Cshape_MIN);
    _duct.set_solidtype(pb::SolidType_MIN);
    _duct.set_htmodel(pb::HeatTransferModel_MIN);
    _duct.set_dragmodel(pb::DragModel_MIN);
    _duct.set_stempfunc("293.15+0*x/L");
    _duct.set_stempmodel(pb::SolidTemperatureModel_MIN);
    
    set(_duct);
    
    TRACE(15,"end AddDuctDialog");
    _blocked = false;
    changed();
}
AddDuctDialog::~AddDuctDialog(){
    delete _dialog;
}
void AddDuctDialog::accept(){
    
    QDialog::accept();
}
void AddDuctDialog::reject(){
    QDialog::reject();
}
void AddDuctDialog::set(const pb::Duct& duct) {
    TRACE(15,"AddDuctDialog::set()");
    _duct = duct;
    _dialog->L->setText(QString::number(duct.length()));
    _dialog->ngp->setText(QString::number(duct.ngp()));
    _dialog->dxb->setText(QString::number(duct.dxb()));
    _dialog->dxmid->setText(QString::number(duct.dxmid()));
    _dialog->S->setText(QString::fromStdString(duct.area()));
    _dialog->phi->setText(QString::fromStdString(duct.phi()));
    _dialog->rh->setText(QString::fromStdString(duct.rh()));
    _dialog->cshape->setCurrentIndex((int) duct.cshape());    
    _dialog->solidtype->setCurrentIndex((int) duct.solidtype());    
    _dialog->gridtype->setCurrentIndex((int) duct.gridtype());
    _dialog->htmodel->setCurrentIndex((int) duct.htmodel());
    _dialog->dragmodel->setCurrentIndex((int) duct.dragmodel());
    _dialog->stempmodel->setCurrentIndex((int) duct.stempmodel());
    _dialog->stempfunc->setText(QString::fromStdString(duct.stempfunc()));
}


void AddDuctDialog::changed(){
    TRACE(15,"AddDuctDialog::changed()");

    // Early return when constructor is still running
    if(_blocked) return;

    _duct.set_length(_dialog->L->text().toFloat());
    _duct.set_ngp(_dialog->ngp->text().toInt());
    _duct.set_dxb(_dialog->dxb->text().toFloat());
    _duct.set_dxmid(_dialog->dxmid->text().toFloat());
    _duct.set_area(_dialog->S->text().toStdString());
    _duct.set_phi(_dialog->phi->text().toStdString());
    _duct.set_rh(_dialog->rh->text().toStdString());
    _duct.set_cshape((pb::Cshape) _dialog->cshape->currentIndex());
    _duct.set_solidtype((pb::SolidType) _dialog->solidtype->currentIndex());
    _duct.set_gridtype((pb::GridType) _dialog->gridtype->currentIndex());
    _duct.set_htmodel((pb::HeatTransferModel) _dialog->htmodel->currentIndex());    
    _duct.set_dragmodel((pb::DragModel) _dialog->dragmodel->currentIndex());
    _duct.set_stempmodel((pb::SolidTemperatureModel) _dialog->stempmodel->currentIndex());    
    _duct.set_stempfunc(_dialog->stempfunc->text().toStdString());

    // Empty the graph
    _plot->graph(0)->setData(QVector<d>(),QVector<d>());

    switch (_duct.gridtype()) {
    case pb::Linear:
        _dialog->ngp->setEnabled(true);        
        _dialog->dxb->setEnabled(false);
        _dialog->dxmid->setEnabled(false);
        break;
    case pb::BlGrid:
        _dialog->ngp->setEnabled(false);        
        _dialog->dxb->setEnabled(true);
        _dialog->dxmid->setEnabled(true);
        break;
    default:
        tasmet_assert(false,"Invalid grid type");
        return;
        break;
    }

    
    PreviewShow pshow = (PreviewShow) _dialog->previewshow->currentIndex();
    
    std::unique_ptr<Geom> geom;
    try {
        geom = std::unique_ptr<Geom>(new Geom(_duct));
    }
    catch(TaSMETError& e) {
        return;
    }
    vd x = geom->x;
    vd y;

    switch (pshow) {
    case CSArea:
        _plot->yAxis->setLabel("S [m^2]");
        y = geom->S;
        break;
    case Porosity:
        _plot->yAxis->setLabel("phi [-]");
        y = geom->phi;
        break;
    case HydraulicRadius:
        _plot->yAxis->setLabel("rh [m]");
        y = geom->rh;
        break;
    case SolidTemperatureFunction:
        _plot->yAxis->setLabel("Ts [K]");
        y = geom->rh;
        break;
    default:
        tasmet_assert(false,"Unhandled PreviewShow case");
        break;
    }

    QVector<d> qx = from_arma(x);
    QVector<d> qy = from_arma(y);

    // Assumes graph 0 exists
    _plot->graph(0)->setData(qx, qy);
    // give the axes some labels:
    _plot->xAxis->setLabel("x [m]");

    // set axes ranges, so we see all data:
    _plot->xAxis->setRange(0, _duct.length());

    d max = arma::max(y);
    d min = arma::min(y);

    _plot->yAxis->setRange(0,max+max/10);
    VARTRACE(15,min);
    VARTRACE(15,max);
    // _plot->yAxis->setRange(0, 1);
    _plot->replot();
    
}

//////////////////////////////////////////////////////////////////////