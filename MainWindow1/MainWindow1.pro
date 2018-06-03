#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T11:15:57
#
#-------------------------------------------------

QT       += core gui
QT += widgets serialport
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = MainWindow1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    ventanagraficopotencia.cpp \
    potenciadatabase.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    ventanagraficopotencia.h \
    potenciadatabase.h

FORMS    += mainwindow.ui \
    ventanagraficopotencia.ui

DISTFILES +=

RESOURCES += \
    resource.qrc
