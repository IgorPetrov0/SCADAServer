#include "clientsocket.h"

clientSocket::clientSocket(QObject *parent):
    QTcpSocket(parent)
{
    connect(this,SIGNAL(disconnected()),this,SLOT(disconnectedSlot()));
    connect(this,SIGNAL(readyRead()),this,SLOT(readClientSlot()));
}
////////////////////////////////////////////////////////////////////////////////
void clientSocket::setIndex(int index){
    socketIndex=index;
}
///////////////////////////////////////////////////////////////////////////////
void clientSocket::disconnectedSlot(){
    deleteLater();
    emit socketDisconnected(socketIndex);
}
////////////////////////////////////////////////////////////////////////////
void clientSocket::readClientSlot(){
    emit dataReady(socketIndex);
}
