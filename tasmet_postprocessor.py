#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Thu Feb  2 08:33:53 2017

@author: anne
"""

# Here we provide the necessary imports.
# The basic GUI widgets are located in QtGui module. 
import sys, h5py
import numpy as np
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import QIcon,QDoubleValidator
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt5 import NavigationToolbar2QT as NavigationToolbar
from matplotlib.figure import Figure
from gui.about_dialog import Ui_about_dialog

scale = [('nano',-9,'n'),
         ('micro',-6,'$\mu$'),
         ('milli:',-3,'m'),
         ('centi',-2,'c'),
         ('deci',-1,'d'),
         ('-',0,''),
         ('deca',1,'da'),
         ('hecto',2,'h'),
         ('kilo',3,'k'),
         ('mega',6,'M'),
         ('giga',9,'G'),
         ('tera',12,'T')]

appname = 'TaSMET Postprocessor'
def getGuiSettings():
    return QSettings(appname,appname)

 
class MaxCounter(object):
    def __init__(self,max=-1):
        self.a = 0
        self.max = max            
    def __call__(self): 
        rval = self.a
        self.a += 1
        if self.a == self.max+1:
            self.a = 0
        return rval

class StepCounter(object):
    def __init__(self,step=1):
        self.a = 0
        self.step = step
        self.inner = 0
        
    def __call__(self): 
        rval = self.a
        
        self.inner += 1
        
        if self.inner == self.step:
            self.inner = 0
            self.a += 1
     
        return rval    

class TaSMETFigure(Figure):
    
    def __init__(self, width=5, height=4, dpi=100):
        
        Figure.__init__(self,figsize=(width, height), dpi=dpi)
        
        self._axes = self.add_subplot(111)

        # We want the axes cleared every time plot() is called
        
        self._plots = []
        self._grid = False
    def addPlot(self,plot,one):
        if one:
            self._plots = [plot]
        else:
            self._plots.append(plot)
        
        self.rePlot()
        
    def rePlot(self):
        self._axes.clear()
        for plot in self._plots:
            self._axes.plot(plot['x'],plot['y'],label=plot['ylabel'])
        
            self._axes.set_ylabel(plot['ylabel'])
            self._axes.set_xlabel(plot['xlabel'])
            
        self._axes.grid(self._grid,'both')
        
        self.canvas.draw_idle()
    def setGrid(self,grid):
        self._grid = grid
        self.rePlot()
        
    def clearLastPlot(self):
        if len(self._plots) > 0:
            del self._plots[-1]
        self.rePlot()
    def clearAllPlot(self):
        self._plots = []
        self.rePlot()

        
class TaSMETCanvas(FigureCanvas):

    def __init__(self, parent, figure):
        
        FigureCanvas.__init__(self, figure)
        self.setParent(parent)

        FigureCanvas.setSizePolicy(self,
                                   QSizePolicy.Expanding,
                                   QSizePolicy.Expanding)
        
        FigureCanvas.updateGeometry(self)


class MainWindow(QMainWindow):
    
    def __init__(self, file_):
        QMainWindow.__init__(self)

        # Open the postprocessing file
        try:
            self._file = h5py.File(file_,'r')
        except:
            QMessageBox.warning(self,
            'Error opening file',
            'File %s is not a valid TaSMET Postprocessing file. Closing.' %file_)
            exit(1)

        # Suppress triggering of the GUI changed() callbacks
        self._suppressed = True

        self.setWindowTitle(appname)
        
        # Window icon
        self.setWindowIcon(QIcon('images/tasmet_small.png'))        
        
        self._figure = TaSMETFigure()
        self._canvas= TaSMETCanvas(self,figure=self._figure)        

        self._seg_box = QComboBox()
        self._seg_box.currentIndexChanged.connect(self.segItemChanged)
        
        self._qty_box = QComboBox()
        self._qty_box.currentIndexChanged.connect(self.qtyItemChanged)
        
        
        # Horizontal offset
        self._hor_offset = QLineEdit()
        self._hor_offset.setAlignment(Qt.AlignRight)
        self._hor_offset.setText('0.0')
        self._hor_offset.setMinimumWidth(40)

        # Vertical offset
        self._ver_offset = QLineEdit()
        self._ver_offset.setAlignment(Qt.AlignRight)
        self._ver_offset.setText('0.0')
        self._ver_offset.setMinimumWidth(40)
                             
        offset_validator = QDoubleValidator()
        self._hor_offset.setValidator(offset_validator)
        self._ver_offset.setValidator(offset_validator)
        
        
        self._scale = QComboBox()
        for val in scale:
            self._scale.addItem(val[0] + ' (%0.0e)' %(10**val[1]))
        self._scale.setCurrentIndex(5)
        
        self._hold_button = QPushButton('Hold on')
        self._hold_button.setCheckable(True)
        
        self._vstime = QRadioButton('Plot vs time')
        self._vstime.clicked.connect(self.qtyItemChanged)
        
        self._vspos = QRadioButton('Plot vs position')
        self._vspos.clicked.connect(self.qtyItemChanged)
        
        self._vspos.setChecked(True)
        self._vs_instance = QSpinBox()
        
        self._plot_button = QPushButton('Plot')
        self._plot_button.clicked.connect(self.plot)

        self._output_button = QPushButton('Output')
        self._output_button.clicked.connect(self.output)
        
        self._animate_button = QPushButton('Animate')
        self._animate_button.clicked.connect(self.animate)
                
        
        top = QGridLayout()


                
        col = StepCounter(step=2)
        row = MaxCounter(max=1)
        
        top.addWidget(QLabel('Segment:'),row(),col())
        top.addWidget(QLabel('Quantity:'),row(),col())
        top.addWidget(self._seg_box,row(),col())
        top.addWidget(self._qty_box,row(),col())
        
        top.addWidget(QLabel('Horizontal offset'),row(),col())
        top.addWidget(QLabel('Vertical offset'),row(),col())

        top.addWidget(self._hor_offset,row(),col())
        top.addWidget(self._ver_offset,row(),col())
              
        top.addWidget(QLabel('Scale:'),row(),col())
        row(); col()
        top.addWidget(self._scale,row(),col())
        row(); col()

        radiobox = QHBoxLayout()
        radiobox.addWidget(self._vstime)
        radiobox.addWidget(self._vspos)

        top.addLayout(radiobox,row(),col())
        
        instance_box = QHBoxLayout()
        instance_box.addWidget(QLabel('Time/position instance:'))
        instance_box.addWidget(self._vs_instance)
        
        top.addLayout(instance_box,row(),col())
        
        top.addWidget(self._plot_button,row(),col())
        top.addWidget(self._hold_button,row(),col())
        
        top.addWidget(self._output_button,row(),col())
        top.addWidget(self._animate_button,row(),col())
        
        htop = QHBoxLayout()
        htop.addLayout(top)
        htop.addStretch()

        vlayout = QVBoxLayout()
        vlayout.addLayout(htop)
        navbar = NavigationToolbar(self._canvas, self)
        
        vlayout.addWidget(navbar)
        vlayout.addWidget(self._canvas)
        
        hlayout = QHBoxLayout()
        
        output_layout = QVBoxLayout()        
        output_layout.addWidget(QLabel('Output'))
        
        self._output = QPlainTextEdit()
        self._output.setFixedWidth(300)
        
        self._output.setStyleSheet('font-family: monospace; background-color: white; color: black;')
        
        output_layout.addWidget(self._output)
        
        hlayout.addLayout(vlayout)        
        hlayout.addLayout(output_layout)

        # Put stuff in a widget and put it in the main window
        widget = QWidget()        
        widget.setLayout(hlayout)
        self.setCentralWidget(widget)
    
        # File menu
        file_menu = QMenu('&File', self)
        file_menu.addAction('&Open',self.loadFile)
        file_menu.addAction('&Exit',self.close)        

        # Plot menu
        plot_menu = QMenu('&Plot', self)
        self._grid_option = QAction('Enable grid')
        self._grid_option.setCheckable(True)
        self._grid_option.triggered.connect(self._figure.setGrid)
        plot_menu.addAction(self._grid_option)
        plot_menu.addAction('&Delete last line',self._figure.clearLastPlot)
        plot_menu.addAction('&Delete all lines',self._figure.clearAllPlot)

        # Options menu
        options_menu = QMenu('&Options', self)
        options_menu.addAction('Clear output',self.clearOutput)
        options_menu.addAction('Default output',self.defaultOutput)
        
        # Help menu
        help_menu = QMenu('&Help', self)
        help_menu.addAction('&About', self.about)
        
        # Add menus to the menubar        
        self.menuBar().addMenu(file_menu)
        self.menuBar().addMenu(plot_menu)
        self.menuBar().addMenu(options_menu)
        self.menuBar().addMenu(help_menu)

        # Set window sizes
        settings = getGuiSettings()
        if settings.value('Geometry') is not None:
            self.restoreGeometry(settings.value("Geometry"))
        
        self.initGui()
        self._suppressed = False
    
    def clearOutput(self):
        self._output.setPlainText('')
    
    def defaultOutput(self):
        txt = ''
        nsegments = 0
        for x in self._file:
            if isinstance(self._file[x], h5py.Group):
                nsegments+=1
        basestr = '{:22}: {:>12} [{:2}]\n'
        txt += basestr.format('Number of segments',nsegments,'-')
        txt += basestr.format('Fundamental frequency',self._file.attrs['freq'][0],'Hz')
        txt += basestr.format('Numer of time samples',self._file.attrs['Ns'][0],'-')
        txt += basestr.format('Reference temperature',self._file.attrs['T0'][0],'K')
        txt += basestr.format('Reference pressure',self._file.attrs['p0'][0],'Pa')
        txt += basestr.format('Gas type',u''+self._file.attrs['gas'].decode('utf-8'),'-')
        tinst = self._file.attrs['t']
        txt += '\nTime instances:\n'
        for i in range(tinst.shape[0]):
            txt += '{:2}: {:2e}\n'.format(i,tinst[i])
            
        
        
        self._output.setPlainText(txt)
    
    def output(self):
        print('output')
    
    def animate(self):
        print('animate')
    
    def getCurrentData(self):
        """
        Returns the current datatype, None if invalid or not selected        
        """
        segitemno = self._seg_box.currentText()
        itemtxt = self._qty_box.currentText()
        if len(segitemno) == 0:
            return None
        if len(itemtxt)==0:
            return None
        else:
            data = self._file[segitemno][itemtxt]
            datatype = data.attrs['datatype'].decode('utf-8')
            return data,datatype
        
  
        
    
    def segItemChanged(self,item):
        """
        Update the combobox with quantities
        """
        self._suppressed = True
        self._qty_box.clear()
        one = False
        for dataset in self._file[str(item)]:
            self._qty_box.addItem(dataset)
            one = True
        
        # Not at least one plottable item in the list of the selected segment
        self._plot_button.setEnabled(one)
        self._output_button.setEnabled(one)
            
        self._suppressed = False
        self._qty_box.setCurrentIndex(0)
        self.qtyItemChanged(0)
    
    def qtyItemChanged(self,item):
        """
        The quantity item has been changed
        """
        
        if self._suppressed: return
        try:
            data,datatype = self.getCurrentData()
        except:
            data = None
        
        segitemno = self._seg_box.currentText()
        
        if data is None:
            self._vstime.setChecked(False)
            self._vspos.setEnabled(False)
            self._vstime.setEnabled(False)
            self._vs_instance.setEnabled(False)
            self._plot_button.setEnabled(False)
            self._output_button.setEnabled(False)
            self._animate_button.setEnabled(False)
            return
        
        self._plot_button.setEnabled(True)
        self._output_button.setEnabled(True)
        
        
        if datatype == 'time':
            self._vstime.setChecked(True)
            self._vspos.setEnabled(False)
            self._vstime.setEnabled(False)
            self._vs_instance.setEnabled(False)
            self._animate_button.setEnabled(False)
            
        elif datatype == 'pos':
            self._vspos.setChecked(True)
            self._vspos.setEnabled(False)
            self._vstime.setEnabled(False)
            self._vs_instance.setEnabled(False)
            self._animate_button.setEnabled(False)
            
        elif datatype == 'postime':
            vspos = self._vspos.isChecked()
            if vspos:
                t = self._file.attrs['t']
                self._vs_instance.setRange(0,len(t)-1)
            else:
                x = self._file[segitemno]['x']                
                self._vs_instance.setRange(0,len(x)-1)
            self._vs_instance.setValue(0)
            
            self._vspos.setEnabled(True)
            self._vstime.setEnabled(True)
            self._vs_instance.setEnabled(True)
        
    def initGui(self):
        self.defaultOutput()
        self._suppressed = True
        for x in self._file:
            if isinstance(self._file[x], h5py.Group):
                self._seg_box.addItem(x)
            else:
                print(self._file[x])
        self._suppressed = False
        self.segItemChanged(0)

    def loadFile(self, file=None):
        pass

    def getx(self):
        """
        Returns the x-axis for the current data selected. Returns None if invalid.
        Returns a tuple containing: x-values, x-legend
        """
        try:
            data,datatype = self.getCurrentData()
        except:
            return None
        
        segitemno = self._seg_box.currentText()
        vspos = self._vspos.isChecked()
        
        x = None
        if datatype == 'time' or (datatype == 'postime' and not vspos):
            x = self._file.attrs['t']
            xsymbol = 't'
            xunit = 's'
        elif datatype == 'pos' or (datatype == 'postime' and vspos):
            xset = self._file[segitemno]['x']
            x = xset[:]
            xsymbol = xset.attrs['symbol'].decode('utf-8')
            xunit = xset.attrs['unit'].decode('utf-8')
        
        xlabel = xsymbol + ' [' + xunit + ']'
        
        return x,xlabel
        
    def gety(self):
        """
        Returns the y-axis for the current data selected. Returns None if invalid.
        Returns a tuple containing x-values, x-legend
        """
        try:
            data,datatype = self.getCurrentData()
        except:
            return None
        
        symbol = data.attrs['symbol'].decode('utf-8')
        unit = data.attrs['unit'].decode('utf-8')
        
        vspos = self._vspos.isChecked()
                                                       
        ylabel = symbol + ' [' + unit + ']'
                            
        if datatype == 'time' or datatype == 'pos':
            y = data[:]
        elif datatype == 'postime':
            data_inst  = self._vs_instance.value()
            if self._vspos.isChecked():
                y = data[:,data_inst]
            else:
                y = data[data_inst,:]
                
        return y,ylabel
        
    
    def plot(self):

        plot = {}

        
        hor_offset = float(self._hor_offset.text())
        ver_offset = float(self._ver_offset.text())
        try:
            x,xlabel = self.getx()
        except TypeError:
            return
        
        try:
            y,ylabel = self.gety()
        except TypeError:
            return
        
        plot['x'] = x+hor_offset
        plot['xlabel'] = xlabel
            
        thescale = scale[self._scale.currentIndex()]
        scaleval = thescale[1]
        scaleylabel = '' if scaleval == 0 else ' x $10^{%i}$' %scaleval
        
        plot['y'] = (y+ver_offset)/(10**scaleval)
        plot['ylabel'] = ylabel + ' ' + scaleylabel
        
        self._figure.addPlot(plot, not self._hold_button.isChecked())
        
    def about(self):
        dialog = QDialog(self)
        about_dialog = Ui_about_dialog()
        about_dialog.setupUi(dialog)
        dialog.exec_()
        
    def closeEvent(self,event):
        settings = getGuiSettings()
        settings.setValue("Geometry", self.saveGeometry())
        
        event.accept()

def usage():
    print("Usage:")
    if hasattr(sys,'frozen'):
        pass
    else:
        print('python ' + sys.argv[0] + ' <file>.tasmet.h5')

if __name__ == '__main__':
    if(len(sys.argv) < 2):
        usage()
        file_ = '../second_duct_sinomgt.tasmet.h5'
#        return -1
    else:
        file_ = sys.argv[1]
        
    # Every PyQt4 application must create an application object.
    # The application object is located in the QtGui module.
#    try:
#        app = QApplication.instance()
#    except:
    app = QApplication(sys.argv)
    
    # The QWidget widget is the base class of all user interface objects in PyQt4.
    # We provide the default constructor for QWidget. The default constructor has no parent.
    # A widget with no parent is called a window. 
#    w = MainWindow(sys.argv[1])
    w = MainWindow(file_)    
    w.resize(640, 480)  # The resize() method resizes the widget.
    
    w.show()  # The show() method displays the widget on the screen.
    
    sys.exit(app.exec_())  # Finally, we enter the mainloop of the application.