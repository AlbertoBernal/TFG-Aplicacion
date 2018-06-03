#ifndef POTENCIADATABASE_H
#define POTENCIADATABASE_H
#include <QPair>
#include <QVector>
#include <QString>
#include <QTime>
#include <algorithm>
#include <vector>
#include <map>
#include <QObject>

class PotenciaDatabase : public QObject
{
    Q_OBJECT
private:
    typedef std::pair<double,QTime> StoredData;
    std::vector< StoredData > Pot_Time_Data;
    int _Amount;
    int _index;
    int _StoredDataLimit;
    bool dataBaseIsOk;
    QString _nombre = "C:/Users/alberto/Documents/MainWindow1/archivo.csv";
#define NUMBER_OF_FIELDS 2
    enum fields{POWER , TIME};

public slots:
    void dataQuery();
public:

    bool leeCSV(const std::string& nombre);
    double averagePowerAtHour(int hour);
    double averagePowerAtDay(int day);
    double averagePowerAtMonth(int month);
    QVector<double> averageMonthsPowerAtYear();
    bool Add (double PotData);
    bool Add (double PotData , QTime time);
    PotenciaDatabase(const int MAXNUMBER,QObject *parent = 0);
    PotenciaDatabase(const int MAXNUMBER ,QString nombre,QObject*parent = 0);
    ~PotenciaDatabase();
    QVector<double> getRawPowerData();
    QVector<QString> getRawTimeData();
    void getRawPowerTimeData(double& potencia,QString& tiempo);
    void getLastPowerTimeData(double& potencia,QString& tiempo);
    bool addDataLine(const QString& ValuesString);
};

#endif // POTENCIADATABASE_H
