#include "netserver.h"

netServer::netServer()
{
    statCorePointer=nullptr;
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
////////////////////////////////////////////////////////////////////////
void netServer::setStatisticCorePointer(statisticCore *pointer){
    statCorePointer=pointer;
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
//////////////////////////////////////////////////////////////////////////////////////////
void netServer::sendStatistic(int index){
    if(statCorePointer!=nullptr){
        QByteArray array;
        QDataStream str(&array,QIODevice::WriteOnly);
        str<<qint64(0);//место для размера
        str<<(uchar)TCP_PACKET_STATISTIC;
        int mashinesArraySize=statCorePointer->getMashinsCount();
        str<<mashinesArraySize;
        for(int n=0;n!=mashinesArraySize;n++){
            statCorePointer->getMashine(n)->netSerialise(&str);
        }
        str.device()->seek(0);
        str<<qint64(array.size());
        socketsArray.at(index)->write(array);
        socketsArray.at(index)->flush();
    }
    else{
        qDebug("netServer::sendStatistic()  statistic core pointer is NULL");
    }
}
//////////////////////////////////////////////////////////////////////////////////////
void netServer::editObject(QDataStream *str, int index){
    QByteArray array;
    QDataStream tmpStr(&array,QIODevice::WriteOnly);
    object tmp;
    tmp.deserialisation(str);

    if(statCorePointer!=nullptr){
        object *objectPointer=statCorePointer->getObjectForName(tmp.getName());
        if(objectPointer==nullptr){//если такого объекта в базе не существует
            tmpStr<<qint64(0);//место для размера
            tmpStr<<(uchar)TCP_PACKET_ERROR;
            tmpStr<<tr("Объекта с именем <")+tmp.getName()+tr("> не существует. \n Изменения не сохранены.");
            tmpStr.device()->seek(0);
            tmpStr<<qint64(array.size());
            socketsArray.at(index)->write(array);
            socketsArray.at(index)->flush();
            return;
        }
        objectPointer->setDescription(tmp.getDescription());//переписываем только разрешенные для редактирования параметры
        objectPointer->setAddress(tmp.getAddress());
        switch(objectPointer->getType()){
            case(objectMashine):{
                mashine *tmpMashine=static_cast<mashine*>(objectPointer);
                tmpMashine->deserialisationContinue(str);
                break;
            }
        }
        tmpStr<<qint64(0);//место для размера
        tmpStr<<(uchar)TCP_PACKET_ANSWER;
        tmpStr<<tr("Объект <")+tmp.getName()+tr("> успешно изменен.");
        tmpStr.device()->seek(0);
        tmpStr<<qint64(array.size());
        socketsArray.at(index)->write(array);
        socketsArray.at(index)->flush();
    }
    else{
        qDebug("netServer::sendStatistic()  statistic core pointer is NULL");
    }
}
////////////////////////////////////////////////////////////////////////////////////
void netServer::decodeCommand(QDataStream *str, int index){
    uchar command=SERVERCOMMAND_NO_COMMAND;
    *str>>command;
    switch(command){
        case(SERVERCOMMAND_GET_STATISTIC):{
            sendStatistic(index);
            break;
        }
        case(SERVERCOMMAND_EDIT_OBJECT):{
            editObject(str,index);
            break;
        }
    }
    incomingBuffer.clear();
}
/////////////////////////////////////////////////////////////////////////
void netServer::deleteSlot(int index){
    socketsArray.remove(index);
}
//////////////////////////////////////////////////////////////////////////////////////
void netServer::dataReadySlot(int index){
    clientSocket *tmpSocket=socketsArray.at(index);
    qint64 packetSize=0;
    incomingBuffer.append(tmpSocket->readAll());
    QDataStream str(incomingBuffer);

    str>>packetSize;
    if(packetSize==incomingBuffer.size()){
        uchar type;
        str>>type;
        switch(type){
            case(TCP_PACKET_COMMAND):{
                decodeCommand(&str,index);
                break;
            }
        }
        incomingBuffer.clear();
    }
    else if(packetSize<incomingBuffer.size()){
        consoleMessage(tr("Неверный формат пакета от клиента ")+tmpSocket->peerAddress().toString());
        incomingBuffer.clear();
    }
}
