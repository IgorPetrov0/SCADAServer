#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>


class clientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    clientSocket(QObject *parent = 0);
    void setIndex(int index);

protected:
    int socketIndex;
    QByteArray data;


protected slots:
    void disconnectedSlot();
    void readClientSlot();

signals:
    void socketDisconnected(int index);
    void dataReady(int index);




};

#endif // CLIENTSOCKET_H
