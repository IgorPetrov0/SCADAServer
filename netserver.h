#ifndef NETSERVER_H
#define NETSERVER_H


#include <QTcpServer>
#include "clientsocket.h"
#include "defines.h"
#include "errorprocessor.h"


class netServer : public QTcpServer, public errorProcessor
{
    Q_OBJECT

public:
    netServer();
    ~netServer();
    void setNetPort(int port);

protected:
    QTcpServer *server;
    virtual void incomingConnection(qintptr socketDescriptor);
    QVector<clientSocket*>socketsArray;

protected slots:
    void deleteSlot(int index);

signals:
    void consoleMessage(QString string);


};

#endif // NETSERVER_H
