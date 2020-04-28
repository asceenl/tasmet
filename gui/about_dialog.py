# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'about_dialog.ui'
#
# Created by: PyQt4 UI code generator 4.12
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_about_dialog(object):
    def setupUi(self, about_dialog):
        about_dialog.setObjectName(_fromUtf8("about_dialog"))
        about_dialog.resize(318, 582)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(about_dialog.sizePolicy().hasHeightForWidth())
        about_dialog.setSizePolicy(sizePolicy)
        self.verticalLayout = QtGui.QVBoxLayout(about_dialog)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.label = QtGui.QLabel(about_dialog)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8("DejaVu Sans Mono"))
        font.setPointSize(14)
        font.setBold(True)
        font.setWeight(75)
        self.label.setFont(font)
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.label_4 = QtGui.QLabel(about_dialog)
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.verticalLayout.addWidget(self.label_4)
        self.label_5 = QtGui.QLabel(about_dialog)
        self.label_5.setAlignment(QtCore.Qt.AlignCenter)
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.verticalLayout.addWidget(self.label_5)
        self.label_2 = QtGui.QLabel(about_dialog)
        self.label_2.setMinimumSize(QtCore.QSize(300, 433))
        self.label_2.setMaximumSize(QtCore.QSize(300, 433))
        self.label_2.setStyleSheet(_fromUtf8("background-color: rgb(255, 255, 255)"))
        self.label_2.setFrameShape(QtGui.QFrame.Box)
        self.label_2.setText(_fromUtf8(""))
        self.label_2.setPixmap(QtGui.QPixmap(_fromUtf8("images/tasmet.png")))
        self.label_2.setScaledContents(True)
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.verticalLayout.addWidget(self.label_2)
        self.label_3 = QtGui.QLabel(about_dialog)
        self.label_3.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.verticalLayout.addWidget(self.label_3)
        self.pushButton = QtGui.QPushButton(about_dialog)
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.verticalLayout.addWidget(self.pushButton)

        self.retranslateUi(about_dialog)
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), about_dialog.accept)
        QtCore.QMetaObject.connectSlotsByName(about_dialog)

    def retranslateUi(self, about_dialog):
        about_dialog.setWindowTitle(_translate("about_dialog", "About TaSMET", None))
        self.label.setText(_translate("about_dialog", "TaSMET", None))
        self.label_4.setText(_translate("about_dialog", "Thermoacoustic System", None))
        self.label_5.setText(_translate("about_dialog", "Modeling Environment Twente", None))
        self.label_3.setText(_translate("about_dialog", "Copyright (c) 2017 Anne de Jong", None))
        self.pushButton.setText(_translate("about_dialog", "OK", None))

