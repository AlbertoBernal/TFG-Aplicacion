#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QtWidgets>
#include <QtSerialPort/QSerialPort>
#include <QWindow>
#include "VentanaGraficoPotencia.h"
#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPortInfo>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    void closeEvent(QCloseEvent *event);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:

    void onOff(bool);
    void activateLed();
    void conectarUART();
    void desconectarUART();
    void visualizarGrafico();
    void readData();
private:
    void setProgressBarColor(int ProgressBarVal);
    void createMenus();
    void createActions();
    void createStatusBar();
    bool OnState;
    QString conected;
    Ui::MainWindow *ui;
    VentanaGraficoPotencia *graficaPotencia;
    QToolBar    *toolBar;
    QMenu       *fileMenu;
    QMenu       *fileMenu2;
    QMenu       *fileMenu3;
    QAction     *conectarUart;
    QAction     *desconectarUart;
    QAction     *VisualizarGrafico;
    QSerialPort *puertoSerie;
    QStatusBar  *barraStatus;
    QLabel      *barraStatusLabel;
    QPalette     progressBarPalette;
};

#endif // MAINWINDOW_H
