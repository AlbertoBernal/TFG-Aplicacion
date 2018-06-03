#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),OnState(false),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //set progressbar to its minumum value
    ui->progressBar->setValue(ui->progressBar->minimum());
    //asociar label con la imagen del led
    QPixmap pix(":/img/luzverde.png");
    ui->label_On->setPixmap(pix);
    int w= ui->label_On->width();
    int h= ui->label_On->height();
    ui->label_On->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QPixmap pix2(":/img/luzroja.png");
    ui->label_Off->setPixmap(pix2);
    int w2= ui->label_Off->width();
    int h2= ui->label_Off->height();
    ui->label_Off->setPixmap(pix2.scaled(w2,h2,Qt::KeepAspectRatio));


    QPixmap pix3(":/img/luzamarilla.png");
    ui->label_ComError->setPixmap(pix3);
    int w3 = ui->label_ComError->width();
    int h3 = ui->label_ComError->height();
    ui->label_ComError->setPixmap(pix3.scaled(w3,h3,Qt::KeepAspectRatio));

    //
    conected="false";
    ui->label_Off->setVisible(!OnState);
    ui->label_On->setVisible(OnState);
    ui->label_ComError->setVisible(false);

    puertoSerie= new QSerialPort(this);

    createActions();
    createMenus();
    createStatusBar();

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(onOff(bool)));
    connect(conectarUart,SIGNAL(triggered()),this,SLOT(conectarUART()));
    connect(desconectarUart,SIGNAL(triggered()),this,SLOT(desconectarUART()));
    connect(VisualizarGrafico,SIGNAL(triggered()),this,SLOT(visualizarGrafico()));
    connect(puertoSerie, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(ui->exitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief MainWindow::closeEvent
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}
/**
 * @brief MainWindow::createStatusBar
 */
void MainWindow::createStatusBar()
{
    barraStatusLabel = new QLabel(this);
    ui->statusBar->addWidget(barraStatusLabel);
    barraStatusLabel->setText(QString(" Conected :  %1").arg(conected));

}
/**
 * @brief MainWindow::createActions
 */
void MainWindow::createActions()
{
    conectarUart = new QAction(tr("conect. UART"),this); //creo la accion con el nombre
    conectarUart->setIcon(QIcon(":/img/conect.png")); //asocio la imagen a la accion
    conectarUart->setEnabled(true);

    desconectarUart = new QAction(tr("descon. UART"),this);
    desconectarUart->setIcon(QIcon(":/img/desconectar.jpg"));
    desconectarUart->setEnabled(false);

    VisualizarGrafico = new QAction(tr("Graf.Pot"),this);
    VisualizarGrafico->setIcon(QIcon(":/img/grafico.png"));

}
/**
 * @brief MainWindow::createMenus
 */
void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("Conectar")); //creo el menu desplegable
    fileMenu->addAction(conectarUart); //añado acciones al menu
    fileMenu->setStyleSheet("QMenu {background-color: #ABABAB;border: 1px solid black;} QMenu::item {background-color: transparent;}");

    fileMenu2=menuBar()->addMenu(tr("Desconectar"));
    fileMenu2->addAction(desconectarUart);
    fileMenu2->setStyleSheet("QMenu {background-color: #ABABAB;border: 1px solid black;} QMenu::item {background-color: transparent;}");

    fileMenu3 = menuBar()->addMenu(tr("Graf.Pot"));
    fileMenu3->addAction(VisualizarGrafico);
    fileMenu3->setStyleSheet("QMenu {background-color: #ABABAB;border: 1px solid black;} QMenu::item {background-color: transparent;}");
}
/**
 * @brief MainWindow::activateLed
 */
void MainWindow::activateLed()
{

}
/**
 * @brief MainWindow::onOff
 * @param a
 */
void MainWindow::onOff(bool a)
{
    OnState = !OnState;
    ui->label_On->setVisible(OnState);
    ui->label_Off->setVisible(!OnState);

}
/**
 * @brief MainWindow::visualizarGrafico
 */
void MainWindow::visualizarGrafico()
{

    graficaPotencia = new VentanaGraficoPotencia(this);
    graficaPotencia->setWindowTitle("Ventana Grafica de Potencias.");
    graficaPotencia->show();


}
/**
 * @brief MainWindow::conectarUART
 */
void MainWindow::conectarUART()
{


    if(!puertoSerie->isOpen())
    {
        conected="true";
        puertoSerie->setPortName("COM4");
        puertoSerie->setBaudRate(QSerialPort::Baud9600);
        puertoSerie->setDataBits(QSerialPort::Data8);
        puertoSerie->setParity(QSerialPort::NoParity);
        puertoSerie->setStopBits(QSerialPort::OneStop);
        puertoSerie->setFlowControl(QSerialPort::NoFlowControl);
        puertoSerie->open(QIODevice::ReadOnly);

        conectarUart->setEnabled(false);
        desconectarUart->setEnabled(true);
        barraStatusLabel->setText(QString(" Conected :  %1").arg(conected));

    }

}
/**
 * @brief MainWindow::desconectarUART
 */
void MainWindow::desconectarUART()
{
    conected="false";
    puertoSerie->close();
    desconectarUart->setEnabled(false);
    conectarUart->setEnabled(true);
}
/**
 * @brief The RawBytes class
 * clase para gestionar el buffer de entrada de el puerto serie
 * //aba[+,-]1345|455/  ; |-> caracter que avisa de que viene el nivel de tension
 */
class RawBytes
{
public:
    enum {MAXBUFFER = 48};
    RawBytes( char FirstControlChar , char SecondControlChar,char StopCharacter, int dataSize,int BufferSize =0) :
        SizeOfBuffer(BufferSize),
        FirstCtrlChar(FirstControlChar),
        SecondCtrlChar(SecondControlChar),
        DataSize(dataSize) ,
        StopChar(StopCharacter)
    {
        NumberOfBytes = 0; State=0;
    }
    ~RawBytes(){}

    int Size() const{return NumberOfBytes;}

    QByteArray GetData()
    {
        if (NumberOfBytes >= SizeOfBuffer){

            for(int i=0;i<ByteStream.size();i++){
                if (State == 0 && ByteStream[i] == FirstCtrlChar){State = 1;FirstCtrlCharPos = i;}
                if (State == 1 && ByteStream[FirstCtrlCharPos + 1] == SecondCtrlChar){
                    data = ByteStream.mid(FirstCtrlCharPos + 2 , ByteStream.indexOf('/'));
                    return data;
                }
                else
                    State = 0;
            }
            ByteStream.right( ByteStream.indexOf('/') );
            if (ByteStream.indexOf('/') == -1)
                Clear();
            data = QByteArray("");
            return data;
        }

    }

    void Clear(){ByteStream.clear();NumberOfBytes=0;State=0; }
    void Append (QByteArray byte){
        ByteStream.append(byte);
        NumberOfBytes = NumberOfBytes + byte.size();
    }

    bool PowerDataisCorrect(const QByteArray& data ,const int numericDataLength) const
    {
        bool isCorrect = true;
        for (int i = 1 ; i< numericDataLength ; i++)
        {
            if (data[i] >= '9' || data[i] <= '0' )
                isCorrect = false;
        }
        return isCorrect;
    }

    bool PowerDataisCorrect() const
    {
        const int numericDataLength = 5;
        bool isCorrect = true;
        for (int i = 1 ; i< numericDataLength ; i++)
        {
            if ((data[i] >= '9' || data[i] <= '0') && data[i] !='-' )
                isCorrect = false;
        }
        return isCorrect;
    }
private:
    int FirstCtrlCharPos;
    int SizeOfBuffer;
    QByteArray ByteStream;
    QByteArray data;
    int NumberOfBytes;
    int DataSize;
    char FirstCtrlChar;
    char SecondCtrlChar;
    char StopChar;
    int State;

};
//get 4 bytes 'a''b''data0' 'data1'
const int BufferSize = 32;
//const int DataSize = 2+1+3+1+3+1;//header(2) + data(1) + pot(3) +separator(1) + busdc(3) + endchar(1)
const int DataSize = 2+1+5+1+3+1;//header(2) + data(1) + pot(5) +separator(1) + busdc(3) + endchar(1)

RawBytes SensorByteStream('a','b','/',DataSize,BufferSize);

const int MAX_DC_BUS_VALUE = 480;
const int MIN_DC_BUS_VALUE = 420;
const int MAX_PROGRESS_BAR_VALUE = 100;
const int MIN_PROGRESS_BAR_VALUE = 0;
/**
 * @brief DC_BUS_TO_PROGRESS_BAR_VALUE
 * @param DC_BUS_VALUE
 * @return
 */
inline int DC_BUS_TO_PROGRESS_BAR_VALUE(int DC_BUS_VALUE)
{

    float slope = (MAX_PROGRESS_BAR_VALUE - MIN_PROGRESS_BAR_VALUE) /(float(MAX_DC_BUS_VALUE - MIN_DC_BUS_VALUE));
    int n = MIN_PROGRESS_BAR_VALUE - slope*MIN_DC_BUS_VALUE;
    int result = (slope*DC_BUS_VALUE + n);

    if (result < 0 )
        result = 0;
    if (result > 100)
        result = 100;
    return result;
}
/**
 * @brief MainWindow::setProgressBarColor
 * @param ProgressBarVal
 */
void MainWindow::setProgressBarColor(int ProgressBarVal)
{
    if (ProgressBarVal <= 100 && ProgressBarVal>= 75)
    {
        QString myStyleSheet = QString(" QProgressBar::chunk { background: green; "
                                       "}");
        QString s = ui->progressBar->styleSheet().append(myStyleSheet);
        ui->progressBar->setStyleSheet(s);
    }
    else if (ProgressBarVal < 75 && ProgressBarVal>= 25 )
    {
        QString myStyleSheet = QString(" QProgressBar::chunk { background: yellow; }");
        QString s = ui->progressBar->styleSheet().append(myStyleSheet);
        ui->progressBar->setStyleSheet(s);
    }
    else if (ProgressBarVal < 25)
    {
        QString myStyleSheet = QString(" QProgressBar::chunk { background: red;}");
        QString s = ui->progressBar->styleSheet().append(myStyleSheet);
        ui->progressBar->setStyleSheet(s);
    }

}

/**
 * @brief MainWindow::readData
 */
void MainWindow::readData()
{
    const char CaracterEncender ='a';
    const char CaracterApagar = 'b';

    if ( SensorByteStream.Size() < BufferSize)
    {
        SensorByteStream.Append(puertoSerie->readAll());
    }

    if (SensorByteStream.Size() >= BufferSize)
    {
        QByteArray data = SensorByteStream.GetData();

        if (!data.isEmpty())
        {
            ui->texto->insertPlainText(QString(data));
            ui->label_ComError->setVisible(!ui->label_ComError->isVisible());

            if(data[0] == CaracterEncender)
            {
                OnState=true;
                ui->label_On->setVisible(OnState);
                ui->label_Off->setVisible(!OnState);
            }
            if(data[0] == CaracterApagar)
            {
                OnState=false;
                ui->label_On->setVisible(OnState);
                ui->label_Off->setVisible(!OnState);
            }
            if (SensorByteStream.PowerDataisCorrect())
            {
                ui->lcdNumber->display( QString (data.mid(1,5)) );
            }
            if (data.indexOf('|') != -1)
            {
                QByteArray Dc_Bus_Val = data.mid(data.indexOf('|') + 1,3);
                //
                //static QString path = qApp->applicationFilePath()+ "/archivo.csv";
                static const QString path = "C:/Users/alberto/Documents/MainWindow1/archivo.csv";
                QFile f(path);
                auto fd = f.open(QIODevice::WriteOnly | QIODevice::Append);
                QTextStream out(&f);

                out << ((Dc_Bus_Val.toStdString() + ';' + QTime::currentTime().toString().toStdString() + '\n').c_str());

                f.close();

                int ProgressBarVal = DC_BUS_TO_PROGRESS_BAR_VALUE( Dc_Bus_Val.toInt() );
                setProgressBarColor(ProgressBarVal);
                ui->progressBar->setValue( ProgressBarVal );
            }

            //clear databuffer
            SensorByteStream.Clear();
        }
    }
}
