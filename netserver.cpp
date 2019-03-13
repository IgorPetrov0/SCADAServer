#include "netserver.h"

netServer::netServer()
{

}
/////////////////////////////////////////////////////////////
netServer::~netServer(){
    int size=socketsArray.size();
    for(int n=0;n!=size;n++){
        if(socketsArray.at(n)!=NULL){
            delete socketsArray.at(n);
        }
    }
}
/////////////////////////////////////////////////////////////
void netServer::setNetPort(int port){
    if(port!=0){
        this->listen(QHostAddress::Any,port);
    }
}
//////////////////////////////////////////////////////////////////////
void netServer::incomingConnection(qintptr socketDescriptor){
    if(socketsArray.size()!=MAX_TCP_CONNECTIONS){
        clientSocket *socket = new clientSocket(this);
        socket->setIndex(socketsArray.size());
        socketsArray.append(socket);
        socket->setSocketDescriptor(socketDescriptor);
          connect(socket,SIGNAL(socketDisconnected(int)),this,SLOT(deleteSlot(int)));
    }
}
/////////////////////////////////////////////////////////////////////////
void netServer::deleteSlot(int index){
    delete socketsArray.at(index);
    socketsArray.remove(index);
}
