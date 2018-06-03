#ifndef VENTANAGRAFICOPOTENCIA_H
#define VENTANAGRAFICOPOTENCIA_H
#include <QTimer>
#include <QDialog>
#include <QStatusBar>
#include "potenciadatabase.h"

namespace Ui {
class VentanaGraficoPotencia;
}

class VentanaGraficoPotencia : public QDialog
{
    Q_OBJECT

private slots:

    void dibuja();
public slots:
    void actualizaGraficas();

public:
    void dibujaGraficaDiaria();
    void dibujaGraficaAnual();
    explicit VentanaGraficoPotencia(QWidget *parent = 0);
    ~VentanaGraficoPotencia();
signals:
    void outOfData();
private:
    Ui::VentanaGraficoPotencia *ui;
    PotenciaDatabase *historicoDePotencias;
    QStatusBar *statusBar;
    QTimer *timer;
    QTime  *clock;
};

#endif // VENTANAGRAFICOPOTENCIA_H
