#ifndef REQUESTCORE_H
#define REQUESTCORE_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QTimer>
#include <QObject>
#include "errorprocessor.h"
#include "defines.h"
#include "statisticcore.h"





class requestCore : public QObject, public errorProcessor
{
    Q_OBJECT
public:
    requestCore(QObject *parent=nullptr);
    ~requestCore();
    QList<QSerialPortInfo> getSerialPortsList();
    QSerialPort *getPort(int number);
    bool setPort(QString name, int number);
    bool readConfiguration(QString workingDir);
    bool writeConfiguration(QString workingDir);
    void setStatisticCorePointer(statisticCore *pointer);


protected:
    QSerialPort *sPort1;
    QSerialPort *sPort2;
    QTimer *requestTimer;
    QTimer *waitTimer;
    statisticCore *statCorePointer;
    int counter;//счетчик объектов
    qint64 inputBytesCounter;
    object *currentObject;
    int reCounter;//счетчик повторных запросов
    bool pass;//два прохода для кольцевого опроса линии
    QTime lastRequestTime;
    unsigned char CRC8(unsigned char *block, unsigned char len);
    unsigned char CRC16(unsigned char *pcBlock, unsigned short len);
    void readPacket();
    unsigned char *generateArray();//для отладки. Массив уничтожает вызывающий
    unsigned char inputArray[1000];//1000 quint16  - это 2000 байт
    QSerialPort *currentPort;
    requestType currentRequest;//текущий запрос
    void waitTimeMashine();//обработка превышенного интервала для машины
    void nextObject();//переключение на опрос следующего объекта
    void requestCurrentObject();
    void reRequestCurrentObject();
    void requestMashine();
    void rGetData();
    void rClear();
    void rSetPorts();

protected slots:
    void requestTime();
    void waitTime();
    void port1DataReadyRead();
    void port2DataReadyRead();
    void port1ErrorSlot(QSerialPort::SerialPortError error);
    void port2ErrorSlot(QSerialPort::SerialPortError error);


signals:
    void consoleMessage(QString string);



};

#endif // REQUESTCORE_H
