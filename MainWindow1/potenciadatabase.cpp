#include "potenciadatabase.h"
#include <utility>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdarg>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

/**
 * @brief PotenciaDatabase::~PotenciaDatabase
 */
PotenciaDatabase::~PotenciaDatabase(){}

/**
 * @brief PotenciaDatabase::dataQuery
 */
void PotenciaDatabase::dataQuery()
{


}
/**
 * @brief PotenciaDatabase::getLastPowerTimeData
 * @param potencia
 * @param tiempo
 */
void PotenciaDatabase::getLastPowerTimeData(double& potencia,QString& tiempo)
{
    QFile f(_nombre);
    if(f.open(QIODevice::ReadOnly))
    {
        QByteArray fileData = f.readAll();
        fileData.count('\n');
        int lastNewLineIndex = fileData.lastIndexOf('\n');
        int previousLastNewLineIndex = fileData.lastIndexOf('\n',lastNewLineIndex - 1);
        QString data = fileData.mid(previousLastNewLineIndex + 1,lastNewLineIndex-previousLastNewLineIndex);

        QStringList dataList = data.split(';');
        potencia = dataList[0].toFloat();
        tiempo    = dataList[1];
        f.close();
    }
}
/**
 * @brief PotenciaDatabase::PotenciaDatabase
 * @param MAXNUMBER
 * @param nombre
 * @param parent
 */
PotenciaDatabase::PotenciaDatabase(const int MAXNUMBER , QString nombre, QObject *parent):QObject(parent)
{
    _Amount = 0;
    _index = 0;
    Pot_Time_Data.reserve(_StoredDataLimit);
    _StoredDataLimit = MAXNUMBER;
    _nombre = nombre;
}
/**
 * @brief PotenciaDatabase::PotenciaDatabase
 * @param MAXNUMBER
 * @param parent
 */
PotenciaDatabase::PotenciaDatabase(const int MAXNUMBER,QObject *parent) :QObject(parent)
{
    _Amount = 0;

    _StoredDataLimit = MAXNUMBER;
    _index = 0;
    Pot_Time_Data.reserve(_StoredDataLimit);
}

/**
 * @brief PotenciaDatabase::addDataLine
 * @param ValuesString
 * @return
 */
bool PotenciaDatabase::addDataLine(const QString& ValuesString)
{
    bool dataInsertionIsCorrect = false;
    QStringList valuesList = ValuesString.split(';');
    if(valuesList.size() == NUMBER_OF_FIELDS){
        //time format hh:mm:ss ej 14:30:32
        dataInsertionIsCorrect = Add(valuesList.at(0).toFloat(),QTime::fromString(valuesList.at(1)));
    }
    return dataInsertionIsCorrect;
}
/**
 * @brief PotenciaDatabase::leeCSV
 * @param nombre
 * @return
 */
bool  PotenciaDatabase::leeCSV(const std::string& nombre)
{
    dataBaseIsOk = false;
    QFile f(nombre.c_str());
    QByteArray fileData;


    if (f.open(QIODevice::ReadOnly))
    {
        fileData = f.readAll();
        QString s(fileData);
        //la coma no la reconoce como el inicio de un numero real , es el punto
        s.replace(',','.');
        QStringList resultStrings = s.split('\n');
        QStringList valuesList;
        bool dataInsertionIsCorrect = false;
        for(QString ValuesString : resultStrings)
        {
            //  valuesList = ValuesString.split(';');

            dataInsertionIsCorrect = addDataLine (ValuesString);

            if (!dataInsertionIsCorrect)
                dataBaseIsOk = dataInsertionIsCorrect;
        }

    }

    if (f.isOpen())
        f.close();
    return dataBaseIsOk;
}

/**
 * @brief PotenciaDatabase::Add
 * @param PotData
 * @param time
 * @return
 */
bool PotenciaDatabase::Add (double  PotData, QTime time)
{
    if (_Amount < _StoredDataLimit && time.isValid()){

        Pot_Time_Data.push_back({PotData,time});
        _Amount++;
        return true;
    }
    return false;
}

/**
 * @brief PotenciaDatabase::Add
 * @param PotData
 * @return
 */
bool PotenciaDatabase::Add (double  PotData)
{
    if (_Amount < _StoredDataLimit){

        Pot_Time_Data.push_back({PotData,QTime::currentTime()});
        _Amount++;
        return true;
    }
    return false;
}

/**
 * @brief The hourFilter struct
 * functor struc to filter powerdata at given hour c++11 allows lambda
 * done by a backward compatibiliy
 */
struct hourFilter
{
    int _hour;
    bool operator()(const std::pair<double,QTime>& data)
    {
        QTime temp = data.second;
        return _hour != temp.hour();}
};

/**
 * @brief The accumulateDoubles struct
 * functor struc to filter powerdata at given hour c++11 allows lambda
 * done by a backward compatibiliy
 */
struct accumulateDoubles
{
#define DELTA  1
    double _sum;
    int _amount;
    accumulateDoubles(double sum,int amount){_sum=sum;_amount = amount;}
    void operator()(const std::pair<double,QTime>& data)
    {
        if (abs(data.first) - DELTA > 0){
            _sum += data.first;
            _amount++;}
    }
};

/**
 * @brief PotenciaDatabase::averagePowerAtHour
 * @param hour
 * @return
 */
double PotenciaDatabase::averagePowerAtHour(int hour)
{
    double sumaTot;

    hourFilter mifilter= {hour};
    accumulateDoubles myaccumulator(0,0);

    std::remove_if(Pot_Time_Data.begin(),Pot_Time_Data.end(),mifilter);

    std::for_each(Pot_Time_Data.begin(),Pot_Time_Data.end(),myaccumulator);

    sumaTot= myaccumulator._sum/myaccumulator._amount;
    return sumaTot;
}
/**
 * @brief PotenciaDatabase::averagePowerAtDay
 * @param day
 * @return
 */
double PotenciaDatabase::averagePowerAtDay(int day)
{
    if(day<32 && day>0)
    {
        /*PROCESS REQUEST*/
    }
}

/**
 * @brief PotenciaDatabase::averagePowerAtMonth
 * @param month
 * @return
 */
double PotenciaDatabase::averagePowerAtMonth(int month)
{
    if(month<13 && month>0)
    {
        /*PROCESS REQUEST*/
    }
}

/**
 * @brief PotenciaDatabase::averageMonthsPowerAtYear
 * Only store one year dataset
 * @return
 */
QVector<double> PotenciaDatabase::averageMonthsPowerAtYear()
{

}
/**
 * @brief PotenciaDatabase::getRawPowerData
 * @return QVector<double>
 */
QVector<double> PotenciaDatabase::getRawPowerData()
{
    QVector<double> powerData(_StoredDataLimit);
    for (StoredData pairOfData : Pot_Time_Data)
    {
        powerData << pairOfData.first;
    }
    return powerData;
}

/**
 * @brief PotenciaDatabase::getRawTimeData
 * @return
 */
QVector<QString> PotenciaDatabase::getRawTimeData()
{
    QVector<QString> timeData(_StoredDataLimit);
    for (StoredData pairOfData : Pot_Time_Data)
    {
        timeData << pairOfData.second.toString();
    }
    return timeData;
}
/**
 * @brief PotenciaDatabase::getRawPowerTimeData
 * @param potencia
 * @param tiempo
 */
void PotenciaDatabase::getRawPowerTimeData(double& potencia,QString& tiempo)
{
    if (_index  < _Amount)
    {
        potencia = Pot_Time_Data[_index].first;
        tiempo   = Pot_Time_Data[_index].second.toString();
        _index++;

    }

}

