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
///////////////////////////////////////////////////////////////////////
bool netServer::writeConfiguration(QString workingDir){
    QFile file(workingDir+"/"+NET_CONFIG_FILE_NAME);
    if(!file.open(QIODevice::WriteOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации сети."));
        return false;
    }
    QDataStream str(&file);
    str<<QString(SIGNATURE);
    str<<(float)CUR_VERSION;
    str<<this->serverPort();

    file.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////
bool netServer::readConfiguration(QString workingDir){
    QFile file(workingDir+"/"+NET_CONFIG_FILE_NAME);
    if(!file.open(QIODevice::ReadOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации сети."));
        return false;
    }
    QDataStream str(&file);
    QString sig;
    str>>sig;
    if(sig!=SIGNATURE){
        setLastError(tr("Ошибка чтения сигнатуры конфигурационного файла сети"));
        file.close();
        return false;
    }
    float v;
    str>>v;//получаем версию
    if(v>CUR_VERSION){
        setLastError(tr("Конфигурационный файл сети создан в болеее новой версии программы."));
        file.close();
        return false;
    }
    qint16 port;
    str>>port;
    this->listen(QHostAddress::Any,port);

    file.close();
    return true;
}
//////////////////////////////////////////////////////////////////////
void netServer::incomingConnection(qintptr socketDescriptor){
    if(socketsArray.size()!=MAX_TCP_CONNECTIONS){
        clientSocket *socket = new clientSocket(this);
        socket->setIndex(socketsArray.size());
        socketsArray.append(socket);
        socket->setSocketDescriptor(socketDescriptor);
          connect(socket,SIGNAL(socketDisconnected(int)),this,SLOT(deleteSlot(int)));
          connect(socket,SIGNAL(dataReady(int)),this,SLOT(dataReadySlot(int)));
    }
}
/////////////////////////////////////////////////////////////////////////
void netServer::deleteSlot(int index){
    delete socketsArray.at(index);
    socketsArray.remove(index);
}
//////////////////////////////////////////////////////////////////////////////////////
void netServer::dataReadySlot(int index){
    clientSocket *tmpSocket=socketsArray.at(index);
    qint64 size;
    size=tmpSocket->bytesAvailable();
    QByteArray array=tmpSocket->read(sizeof(qint64));
    QDataStream str(array);

    str>>size;
    if(size==tmpSocket->bytesAvailable()){
        array.clear();
        array=tmpSocket->readAll();
        uchar command=SERVERCOMMAND_NO_COMMAND;
        str>>command;
        switch(command){
            case(SERVERCOMMAND_NO_COMMAND):{
                return;
            }
            case(SERVERCOMMAND_GET_STATISTIC):{

                break;
            }
        }
    }
}
