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
    str.setVersion(DATA_STREAM_VERSION);
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
    str.setVersion(DATA_STREAM_VERSION);
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
    clientSocket *socket=socketsArray.at(index);
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
        socket->write(array);
        socket->flush();
    }
    else{
        qDebug("netServer::sendStatistic()  statistic core pointer is NULL");
    }
}
////////////////////////////////////////////////////////////////////////////////////////
void netServer::sendAnswer(QString answer, int index){
    clientSocket *socket=socketsArray.at(index);
    QByteArray array;
    QDataStream tmpStr(&array,QIODevice::WriteOnly);
    tmpStr<<qint64(0);//место для размера
    tmpStr<<(uchar)TCP_PACKET_ANSWER;
    tmpStr<<answer;
    tmpStr.device()->seek(0);
    tmpStr<<qint64(array.size());
    socket->write(array);
    socket->flush();
}
////////////////////////////////////////////////////////////////////////////////////////
void netServer::sendError(QString error, int index){
    clientSocket *socket=socketsArray.at(index);
    QByteArray array;
    QDataStream tmpStr(&array,QIODevice::WriteOnly);
    tmpStr<<qint64(0);//место для размера
    tmpStr<<(uchar)TCP_PACKET_ERROR;
    tmpStr<<error;
    tmpStr.device()->seek(0);
    tmpStr<<qint64(array.size());
    socket->write(array);
    socket->flush();
}
//////////////////////////////////////////////////////////////////////////////////////
void netServer::editObject(QDataStream *str, int index){
    clientSocket *socket=socketsArray.at(index);
    object tmp;
    tmp.deserialisation(str);

    if(statCorePointer!=nullptr){
        object *objectPointer=statCorePointer->getObjectForName(tmp.getName());
        if(objectPointer==nullptr){//если такого объекта в базе не существует
            sendError(tr("Объекта с именем <")+tmp.getName()+tr("> не существует. \n Изменения не сохранены."),index);
            emit consoleMessage(tr("Клиент <")+socket->peerAddress().toString()+tr("запросил изменение несуществующего объекта ")+tmp.getName());
            return;
        }
        objectPointer->setDescription(tmp.getDescription());//переписываем только разрешенные для редактирования параметры
        objectPointer->setAddress(tmp.getAddress());
        switch(objectPointer->getType()){
            case(objectMashine):{
                mashine *tmpMashine=static_cast<mashine*>(objectPointer);
                QString path=tmpMashine->getPathForStatistics();//сделано для того, чтобы не давать клиенту путь на диске сервера
                tmpMashine->deserialisationContinue(str);
                tmpMashine->setPathForStatistics(path);
                break;
            }
        }
        sendAnswer(tr("Объект <")+tmp.getName()+tr("> успешно изменен."),index);
        emit consoleMessage(tr("Объект <")+tmp.getName()+tr("> изменен клиентом ")+socket->peerAddress().toString());
    }
    else{
        qDebug("netServer::sendStatistic()  statistic core pointer is NULL");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void netServer::createObject(QDataStream *str, int index){
    clientSocket *socket=socketsArray.at(index);

    if(statCorePointer!=nullptr){
        QByteArray array;
        *str>>array;
        QDataStream objectStr(&array,QIODevice::ReadOnly);

        //дополнительно проверяем на наличие такого объекта в базе
        object tmpObject;
        tmpObject.deserialisation(&objectStr);
        if(statCorePointer->getObjectForName(tmpObject.getName())!=nullptr){
            sendAnswer(tr("Объект с именем ")+tmpObject.getName()+tr(" уже существует."),index);
            return;
        }
        if(statCorePointer->getObjectForAddress(tmpObject.getAddress())!=nullptr){
            sendAnswer(tr("Объект с адресом ")+tmpObject.getAddress()+tr(" уже существует."),index);
            return;
        }
        objectStr.device()->seek(0);

        statCorePointer->createObject(&objectStr,true);
        sendAnswer(tr("Объект создан"),index);
        emit consoleMessage(tr("Создан новый объект по запросу клиента ")+socket->peerAddress().toString());
    }
    else{
        qDebug("netServer::sendStatistic()  statistic core pointer is NULL");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void netServer::deleteObject(QDataStream *str, int index){
    clientSocket *socket=socketsArray.at(index);

    if(statCorePointer!=nullptr){
        QString name;
        *str>>name;
        qint8 all=0;
        *str>>all;//признак удаление всей информации
        object *existObject=statCorePointer->getObjectForName(name);
        if(existObject==nullptr){
            sendAnswer(tr("Объект с именем ")+name+tr(" не существует."),index);
            return;
        }
        if(all==0){
            statCorePointer->deleteObject(existObject,true);
            sendAnswer(tr("Объект удален"),index);
            emit consoleMessage(tr("Объект удален по запросу клиента ")+socket->peerAddress().toString());
        }
        else{
            statCorePointer->deleteObject(existObject,false);
            sendAnswer(tr("Объект и все его данные удалены"),index);
            emit consoleMessage(tr("Объект и все его данные удалены по запросу клиента ")+socket->peerAddress().toString());
        }
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
        case(SERVERCOMMAND_CREATE_OBJECT):{
            createObject(str,index);
            break;
        }
        case(SERVERCOMMAND_DELETE_OBJECT):{
            deleteObject(str,index);
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
