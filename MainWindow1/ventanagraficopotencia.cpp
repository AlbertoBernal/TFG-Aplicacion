#include "ventanagraficopotencia.h"
#include "ui_ventanagraficopotencia.h"
#include <qcustomplot.h>

/**
 * @brief VentanaGraficoPotencia::VentanaGraficoPotencia
 * @param parent
 */
VentanaGraficoPotencia::VentanaGraficoPotencia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaGraficoPotencia)
{
    ui->setupUi(this);
    this->historicoDePotencias = new PotenciaDatabase(1700);
    bool dataBaseOk = this->historicoDePotencias->leeCSV("C:/Users/alberto/Documents/MainWindow1/archivo.csv");
    this->timer     = new QTimer(this);
    this->clock     = new QTime(QTime::currentTime());
    this->statusBar = new QStatusBar(this);
    //SIGNAL & SLOTS
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(dibuja()));
    connect(this,SIGNAL(outOfData()),this->historicoDePotencias,SLOT(dataQuery()));
    clock->start();


}

/**
 * @brief VentanaGraficoPotencia::actualizaGraficas
 */
void VentanaGraficoPotencia::actualizaGraficas()
{
    double key = clock->elapsed()/1000.0;

    double potencia = 0;
    QString tiempo;
    historicoDePotencias->getLastPowerTimeData(potencia,tiempo);
    // add data to lines:
    ui->GraficaPotencia->graph(0)->addData(key, potencia);

    // rescale value (vertical) axis to fit the current data:
    ui->GraficaPotencia->graph(0)->rescaleValueAxis();

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->GraficaPotencia->xAxis->setRange(key, 12, Qt::AlignRight);
    ui->GraficaPotencia->replot();
}
/**
 * @brief VentanaGraficoPotencia::~VentanaGraficoPotencia
 */
VentanaGraficoPotencia::~VentanaGraficoPotencia()
{
    delete ui;
}
/**
 * @brief VentanaGraficoPotencia::dibujaGraficaAnual
 */
void VentanaGraficoPotencia::dibujaGraficaAnual()
{

    //ui->GraficaPotencia->replot();
    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->GraficaPotencia->setBackground(QBrush(gradient));

    // create empty bar chart objects:
    // QCPBars *regen = new QCPBars(ui->GraficaPotencia->xAxis, ui->GraficaPotencia->yAxis);
    QCPBars *nuclear = new QCPBars(ui->GraficaPotencia->xAxis, ui->GraficaPotencia->yAxis);
    QCPBars *fossil = new QCPBars(ui->GraficaPotencia->xAxis, ui->GraficaPotencia->yAxis);
    //   regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    nuclear->setAntialiased(false);
    fossil->setAntialiased(false);
    //regen->setStackingGap(1);
    nuclear->setStackingGap(1);
    fossil->setStackingGap(1);
    // set names and colors:
    fossil->setName(tr("Inyectada a la Red"));
    fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    fossil->setBrush(QColor(111, 9, 176));
    nuclear->setName(tr("Demandada de la Red"));
    nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
    nuclear->setBrush(QColor(250, 170, 20));
    // stack bars on top of each other:
    nuclear->moveAbove(fossil);

    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12;
    labels << "Enero" << "Febrero" << "Marzo" << "Abril" << "Mayo" << "Junio" << "Julio" << "Agosto" << "Septiembre" << "Octubre" << "Noviembre" << "Diciembre";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->GraficaPotencia->xAxis->setTicker(textTicker);
    ui->GraficaPotencia->xAxis->setTickLabelRotation(60);
    ui->GraficaPotencia->xAxis->setSubTicks(false);
    ui->GraficaPotencia->xAxis->setTickLength(0, 4);
    ui->GraficaPotencia->xAxis->setRange(0, 13);
    ui->GraficaPotencia->xAxis->setBasePen(QPen(Qt::white));
    ui->GraficaPotencia->xAxis->setTickPen(QPen(Qt::white));
    ui->GraficaPotencia->xAxis->grid()->setVisible(true);
    ui->GraficaPotencia->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->GraficaPotencia->xAxis->setTickLabelColor(Qt::white);
    ui->GraficaPotencia->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
    ui->GraficaPotencia->yAxis->setRange(0, 850.0);
    ui->GraficaPotencia->yAxis->setPadding(5); // a bit more space to the left border
    ui->GraficaPotencia->yAxis->setLabel("Energia en kWh");
    ui->GraficaPotencia->yAxis->setBasePen(QPen(Qt::white));
    ui->GraficaPotencia->yAxis->setTickPen(QPen(Qt::white));
    ui->GraficaPotencia->yAxis->setSubTickPen(QPen(Qt::white));
    ui->GraficaPotencia->yAxis->grid()->setSubGridVisible(true);
    ui->GraficaPotencia->yAxis->setTickLabelColor(Qt::white);
    ui->GraficaPotencia->yAxis->setLabelColor(Qt::white);
    ui->GraficaPotencia->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->GraficaPotencia->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> fossilData, nuclearData;//, regenData;
    fossilData  << QVector<double>{0.86*850 , 0.83*800 , 0.84*750 , 0.52*650 , 0.55*600 , 0.33*550 , 0.67*550 , 550*0.67 , 600*0.67 , 650*0.67  , 670 *0.67,700*0.67};
    //fossilData  << 0.86*850 << 0.83*800 << 0.84*750 << 0.52*650 << 0.55*600 << 0.33*550 << 0.67*550 << 550*0.67 << 600*0.67 << 650*0.67  << 670 *0.67 << 700*0.67;
    nuclearData   << 0.14*850 << 0.17*800 << 0.16*750 << 0.48*650 << 0.45*600 << 0.66*550 << 0.23*550 << 550*0.23 << 600*0.23 << 650*0.33  << 670 *0.33 << 700*0.33;
    // regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;
    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    // regen->setData(ticks, regenData);

    // setup legend:
    ui->GraficaPotencia->legend->setVisible(true);
    ui->GraficaPotencia->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
    ui->GraficaPotencia->legend->setBrush(QColor(255, 255, 255, 100));
    ui->GraficaPotencia->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->GraficaPotencia->legend->setFont(legendFont);
    ui->GraficaPotencia->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->GraficaPotencia->replot();


}
/**
 * @brief VentanaGraficoPotencia::dibujaGraficaDiaria
 */
void VentanaGraficoPotencia::dibujaGraficaDiaria()
{
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->GraficaPotencia->setBackground(QBrush(gradient));

    ui->GraficaPotencia->addGraph(); // blue line
    ui->GraficaPotencia->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->GraficaPotencia->yAxis->setPadding(5); // a bit more space to the left border
    ui->GraficaPotencia->yAxis->setLabel("Tension Baterias");
    ui->GraficaPotencia->yAxis->setTickLabelColor(Qt::white);
    ui->GraficaPotencia->yAxis->setLabelColor(Qt::white);
    ui->GraficaPotencia->xAxis->setTickLabelColor(Qt::white);
    ui->GraficaPotencia->xAxis->setLabelColor(Qt::white);


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->GraficaPotencia->xAxis->setTicker(timeTicker);
    ui->GraficaPotencia->axisRect()->setupFullAxesBox();
    ui->GraficaPotencia->yAxis->setRange(450, 500);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->GraficaPotencia->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->GraficaPotencia->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->GraficaPotencia->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->GraficaPotencia->yAxis2, SLOT(setRange(QCPRange)));

}
/**
 * @brief VentanaGraficoPotencia::dibuja
 */
void VentanaGraficoPotencia::dibuja()
{
    int action = ui->comboBox->currentIndex();

    switch (action)
    {
    case 0:
        //limpia las graficas
        ui->GraficaPotencia->clearGraphs();
        ui->GraficaPotencia->clearItems();
        ui->GraficaPotencia->clearPlottables();
        if (ui->GraficaPotencia->graphCount() > 0)
            ui->GraficaPotencia->graph()->data()->clear();
        //dibuja la grafica
        this->dibujaGraficaDiaria();
        //actualiza las grafica cada 3 segundos para evitar overhead
        connect(this->timer , SIGNAL(timeout()) , this,SLOT(actualizaGraficas()));
        timer->start(100);
        break;
    case 1:
        ui->GraficaPotencia->clearGraphs();
        ui->GraficaPotencia->clearPlottables();
        ui->GraficaPotencia->clearItems();
        if (ui->GraficaPotencia->graphCount() > 0)
            ui->GraficaPotencia->graph()->data()->clear();
        this->dibujaGraficaAnual();
        //No actualices mas la grafica porque la has quitado del foco
        disconnect(this->timer , SIGNAL(timeout()) , this,SLOT(actualizaGraficas()));

        break;

    };
}

