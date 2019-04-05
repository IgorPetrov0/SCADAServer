#ifndef NETSERVER_H
#define NETSERVER_H


#include <QTcpServer>
#include <QFile>
#include <QDataStream>
#include "clientsocket.h"
#include "defines.h"
#include "errorprocessor.h"
#include "statisticcore.h"


class netServer : public QTcpServer, public errorProcessor
{
    Q_OBJECT

public:
    netServer();
    ~netServer();
    void setNetPort(int port);
    void setStatisticCorePointer(statisticCore *pointer);
    bool writeConfiguration(QString workingDir);
    bool readConfiguration(QString workingDir);

protected:
    statisticCore *statCorePointer;
    QByteArray incomingBuffer;
    virtual void incomingConnection(qintptr socketDescriptor);
    QVector<clientSocket*>socketsArray;
    void sendStatistic(int index);
    void sendAnswer(QString answer, int index);
    void sendError(QString error, int index);
    void editObject(QDataStream *str, int index);
    void createObject(QDataStream *str, int index);
    void deleteObject(QDataStream *str, int index);
    void decodeCommand(QDataStream *str, int index);

protected slots:
    void deleteSlot(int index);
    void dataReadySlot(int index);

signals:
    void consoleMessage(QString string);


};

#endif // NETSERVER_H
