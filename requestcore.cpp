#include "requestcore.h"

requestCore::requestCore(QObject *parent):
    QObject(parent)
{
    sPort1=NULL;
    sPort2=NULL;
    requestTimer = new QTimer(this);
    requestTimer->setSingleShot(true);
    waitTimer = new QTimer(this);
    waitTimer->setSingleShot(true);
    pass=true;
    reCounter=0;
    inputBytesCounter=0;
    currentRequest=REQUEST_EMPTY;
    connect(requestTimer,SIGNAL(timeout()),this,SLOT(requestTime()));
    connect(waitTimer,SIGNAL(timeout()),this,SLOT(waitTime()));
}
///////////////////////////////////////////////////////////////////////////
requestCore::~requestCore()
{
    if(sPort1!=NULL){
        delete sPort1;
    }
    if(sPort2!=NULL){
        delete sPort2;
    }
    delete requestTimer;
}
///////////////////////////////////////////////////////////////////////////////////
QList<QSerialPortInfo> requestCore::getSerialPortsList(){
    return QSerialPortInfo::availablePorts();
}
///////////////////////////////////////////////////////////////////////////////////
QSerialPort *requestCore::getPort(int number){
    switch(number){
        case(1):{
            return sPort1;
        }
        case(2):{
            return sPort2;
        }
        default:{
            qDebug("requestCore::getPort : Argument <number> may be 1 or 2");
            return NULL;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////
bool requestCore::setPort(QString name, int number){
    switch(number){
        case(1):{
            if(sPort1!=NULL){
                sPort1->close();
                disconnect(sPort1,SIGNAL(readyRead()),this,SLOT(port1DataReadyRead()));
                disconnect(sPort1,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port1ErrorSlot(QSerialPort::SerialPortError)));
                delete sPort1;
                sPort1=NULL;
            }
            if((name!="None") & (!name.isEmpty())){
                sPort1 = new QSerialPort(name);
                connect(sPort1,SIGNAL(readyRead()),this,SLOT(port1DataReadyRead()));
                connect(sPort1,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port1ErrorSlot(QSerialPort::SerialPortError)));
                if(!sPort1->open(QIODevice::ReadWrite)){
                    setLastError(tr("Невозможно открыть порт ")+sPort1->portName()+tr(" для чтения и записи."));
                    return false;
                }
                else{
                    emit consoleMessage(tr("Порт 1 ")+sPort1->portName()+ tr(" открыт."));
                }
            }
            break;
        }
        case(2):{
            if(sPort2!=NULL){
                sPort2->close();
                disconnect(sPort2,SIGNAL(readyRead()),this,SLOT(port2DataReadyRead()));
                disconnect(sPort2,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port2ErrorSlot(QSerialPort::SerialPortError)));
                delete sPort2;
                sPort2=NULL;
            }
            if((name!="None") & (!name.isEmpty())){
                sPort2 = new QSerialPort(name);
                connect(sPort2,SIGNAL(readyRead()),this,SLOT(port2DataReadyRead()));
                connect(sPort2,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port2ErrorSlot(QSerialPort::SerialPortError)));
                if(!sPort2->open(QIODevice::ReadWrite)){
                    setLastError(tr("Невозможно открыть порт ")+sPort2->portName()+tr(" для чтения и записи."));
                    return false;
                }
                else{
                    emit consoleMessage(tr("Порт 2 ")+sPort2->portName()+ tr(" открыт."));
                }
            }
            break;
        }
        default:{
            qDebug("requestCore::setPort : Argument <number> may be 1 or 2");
            return false;
        }
    }
    requestTimer->start(REQUEST_TIME);
    emit consoleMessage(tr("Опрос начат."));
    return true;
}
///////////////////////////////////////////////////////////////////////////////////
bool requestCore::readConfiguration(QString workingDir){
    int emptyPorts=0;
    QFile file(workingDir+"/"+SP_CONFIG_FILE_NAME);
    if(!file.open(QIODevice::ReadOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации последовательных портов."));
        return false;
    }
    QDataStream str(&file);
    QString sig;
    str>>sig;
    if(sig!=SPSIGNATURE){
        setLastError(tr("Ошибка чтения сигнатуры конфигурационного файла последовательных портов."));
        file.close();
        return false;
    }
    float v;
    str>>v;//получаем версию
    if(v>CUR_VERSION){
        setLastError(tr("Конфигурационный файл создан в болеее новой версии программы."));
        file.close();
        return false;
    }
    QString tmp;
    str>>tmp;
    emit consoleMessage(tr("Конфигурация ядра опроса прочитана из файла.")+workingDir+"/"+SP_CONFIG_FILE_NAME);
    if(tmp!="None"){
        sPort1 = new QSerialPort(tmp);
        connect(sPort1,SIGNAL(readyRead()),this,SLOT(port1DataReadyRead()));
        connect(sPort1,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port1ErrorSlot(QSerialPort::SerialPortError)));
        if(!sPort1->open(QIODevice::ReadWrite)){
            setLastError(tr("Невозможно открыть порт ")+sPort1->portName()+tr(" для чтения и записи. Опрос остановлен."));
            file.close();
            return false;
        }
        qint32 br;
        str>>br;
        sPort1->setBaudRate(br);
        int tmpInt;
        str>>tmpInt;
        sPort1->setDataBits((QSerialPort::DataBits)tmpInt);
        str>>tmpInt;
        sPort1->setParity((QSerialPort::Parity)tmpInt);
        str>>tmpInt;
        sPort1->setStopBits((QSerialPort::StopBits)tmpInt);
        str>>tmpInt;
        sPort1->setFlowControl((QSerialPort::FlowControl)tmpInt);
    }
    else{
        emptyPorts++;
    }
    str>>tmp;
    if(tmp!="None"){
        sPort2 = new QSerialPort(tmp);
        connect(sPort2,SIGNAL(readyRead()),this,SLOT(port2DataReadyRead()));
        connect(sPort2,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(port2ErrorSlot(QSerialPort::SerialPortError)));
        if(!sPort2->open(QIODevice::ReadWrite)){
            setLastError(tr("Невозможно открыть порт ")+sPort2->portName()+tr(" для чтения и записи."));
            file.close();
            return false;
        }
        qint32 br;
        str>>br;
        sPort2->setBaudRate(br);
        int tmpInt;
        str>>tmpInt;
        sPort2->setDataBits((QSerialPort::DataBits)tmpInt);
        str>>tmpInt;
        sPort2->setParity((QSerialPort::Parity)tmpInt);
        str>>tmpInt;
        sPort2->setStopBits((QSerialPort::StopBits)tmpInt);
        str>>tmpInt;
        sPort2->setFlowControl((QSerialPort::FlowControl)tmpInt);
    }
    else{
        emptyPorts++;
    }
    if(emptyPorts==2){
        setLastError(tr("Последовательные порты не заданы. Опрос по сети RS485 невозможен. \n "
                        "Задайте хотя бы один последовательный порт."));
        file.close();
        return false;
    }
    file.close();
    requestTimer->start(REQUEST_TIME);
    emit consoleMessage(tr("Опрос начат."));
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
bool requestCore::writeConfiguration(QString workingDir){
    QFile file(workingDir+"/"+SP_CONFIG_FILE_NAME);
    if(!file.open(QIODevice::WriteOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации последовательных портов."));
        return false;
    }
    QDataStream str(&file);
    str<<QString(SPSIGNATURE);
    str<<(float)CUR_VERSION;
    if(sPort1!=NULL){
        str<<sPort1->portName();
        str<<(qint32)sPort1->baudRate();
        str<<(int)sPort1->dataBits();
        str<<(int)sPort1->parity();
        str<<(int)sPort1->stopBits();
        str<<(int)sPort1->flowControl();
    }
    else{
        str<<QString("None");
    }
    if(sPort2!=NULL){
        str<<sPort2->portName();
        str<<(qint32)sPort2->baudRate();
        str<<(int)sPort2->dataBits();
        str<<(int)sPort2->parity();
        str<<(int)sPort2->stopBits();
        str<<(int)sPort2->flowControl();
    }
    else{
        str<<QString("None");
    }
    file.close();
    emit consoleMessage(tr("Конфигурация ядра опроса записана в файл .")+workingDir+"/"+SP_CONFIG_FILE_NAME);
    return true;
}
///////////////////////////////////////////////////////////////////////////
void requestCore::setStatisticCorePointer(statisticCore *pointer){
    requestTimer->stop();
    waitTimer->stop();
    statCorePointer=pointer;
    counter=0;
    currentObject=nullptr;
}
///////////////////////////////////////////////////////////////////////////////////
void requestCore::requestCurrentObject(){
    if(currentRequest==REQUEST_EMPTY){
        return;
    }
    switch(currentRequest){
        case(REQUEST_CONFIG_PORTS):{
            inputBytesCounter=0;
            currentPort->clear();

            break;
        }
        case(REQUEST_GET_DATA):{
            inputBytesCounter=0;
            currentPort->clear();
            switch(currentObject->getType()){
                case(objectMashine):{
                    requestMashine(currentRequest);
                    break;
                }
                default:{
                    qDebug("requestCore::requestCurrentObject Wrong object type. ");
                    return;
                }
            }
            waitTimer->start(WAIT_TIME);
            break;
        }
        default:{
            return;
        }

    }

}
/////////////////////////////////////////////////////////////////////////////////
void requestCore::reRequestCurrentObject(){
    reCounter++;
    if(reCounter==MAX_RE_REQUEST){
        reCounter=0;
        nextDevice();
        return;
    }
    requestCurrentObject();
    emit consoleMessage(tr("Повторный запрос данных через ")+currentPort->portName()+tr(" по адресу ")+QString::number(currentObject->getAddress()));
}
//////////////////////////////////////////////////////////////////////////////
void requestCore::requestMashine(requestType request){
    if(currentPort->isOpen()){
        QByteArray array;
        //формируем запрос
        QDataStream str(&array,QIODevice::WriteOnly);
        unsigned char size=4;//размер пакета
        str<<size;
        str<<(unsigned char)currentObject->getAddress();//адрес сетевого объекта
        str<<(unsigned char)request;//команда
        str<<(unsigned char)CRC16((unsigned char*)array.data(),array.size());//контрольная сумма
        currentPort->write(array);
    }
}
/////////////////////////////////////////////////////////////////////////////
unsigned char requestCore::CRC8(unsigned char *block, unsigned char len){
    unsigned char crc = 0xFF;
    unsigned int i;
    while (len--){
        crc ^= *block++;
        for (i = 0; i < 8; i++){
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
    }
    return crc;
}
////////////////////////////////////////////////////////////////////////////
unsigned char requestCore::CRC16(unsigned char *pcBlock, unsigned short len){
    /*
      Name  : CRC-16 CCITT
      Poly  : 0x1021    x^16 + x^12 + x^5 + 1
      Init  : 0xFFFF
      Revert: false
      XorOut: 0x0000
      Check : 0x29B1 ("123456789")
      MaxLen: 4095 байт (32767 бит) - обнаружение
        одинарных, двойных, тройных и всех нечетных ошибок
    */
    unsigned short crc = 0xFFFF;
    unsigned char i;
    while(len--){
        crc^= *pcBlock++ << 8;
        for(i = 0;i<8;i++){
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
        }
    }
    return crc;
}
///////////////////////////////////////////////////////////////////////////////////
void requestCore::readPacket(){
    switch(currentObject->getType()){
        case(objectMashine):{
            mashine *tmpMashine=static_cast<mashine*>(currentObject);
            tmpMashine->readPacket(inputArray,lastRequestTime);
            break;
        }
        default:{
            int t=0;
        }
    }
}
////////////////////////////////////////////////////////////////////////////
void requestCore::nextDevice(){
    //функция должна гарантировать, что или currentObject!=nullptr или опрос остановлен
    inputBytesCounter=0;
    currentPort->clear();
    currentObject=statCorePointer->getObjectForIndex(counter);//пробуем получить объект
    if(currentObject!=nullptr){//если объект не нулевой
        currentRequest=REQUEST_CONFIG_PORTS;
        requestCurrentObject();//то делаем запрос в штатном режиме
        emit consoleMessage(tr("Запрос данных через ")+currentPort->portName()+tr(" по адресу ")+QString::number(currentObject->getAddress()));
        counter++;//инкремент счетчика
    }
    else if((currentObject==nullptr)&&(counter==0)){//если объект нулевой по нулевому индексу
        counter=0;//то ловить нечего- база пуста. останавливаем опрос
        consoleMessage(tr("В базе данных нет ни одного объекта. Опрос остановлен."));
        return;
    }
    else{//если объект просто нулевой, то
        counter=0;//рекурсивно пробуем нулевой индекс
        nextDevice();//или сделает запрос или остановит опрос
    }
}
//////////////////////////////////////////////////////////////////////////
void requestCore::requestTime(){ 
    lastRequestTime=QTime::currentTime();
    currentPort=nullptr;
    if(pass){//если первый проход
        if(sPort1!=nullptr){//в идеале работаем через первый порт
            currentPort=sPort1;
        }
        else if(sPort2!=nullptr){//но на худой конец через второй
            currentPort=sPort2;
        }
        else{//если ни одного живого порта
            consoleMessage(tr("Ни одного активного порта не обнаружено. Опрос остановлен."));
            return;//то выходим
        }
    }
    else{//если второй проход
        if(sPort2!=nullptr){//в идеале работаем через второй порт
            currentPort=sPort2;
        }
        else if(sPort1!=nullptr){//но на худой конец через второй
            currentPort=sPort1;
        }
        else{//если ни одного живого порта
            consoleMessage(tr("Ни одного активного порта не обнаружено. Опрос остановлен."));
            return;//то выходим
        }
    }
    if(counter==0){//меняем проход только при переходе через 0 индекс
        pass^=true;
    }
    nextDevice();
}
///////////////////////////////////////////////////////////////////////////////
void requestCore::waitTime(){
    //сюда попадаем, если время ожидания ответа от устройства превысило WAIT_TIME

    switch(currentObject->getType()){
        case(objectMashine):{
            waitTimeMashine();
            break;
        }
    }
    currentObject->setOnline(false);
    if(inputBytesCounter!=0){//если на момент превышения времени ожидания во входном массиве есть данные

    }  
    currentRequest=REQUEST_EMPTY;
    emit consoleMessage(tr("Превышено время ожидания ответа от ")+currentObject->getName()+tr(". Адрес ")+
                        QString::number(currentObject->getAddress())+tr(". Порт ")+currentPort->portName());
    nextDevice();//переходим к следующему устройству
}
/////////////////////////////////////////////////////////////////////////
void requestCore::port1DataReadyRead(){
    inputBytesCounter+=sPort1->read((char*)&inputArray[inputBytesCounter],2000);//размер пакета ограничен 2000 байт
    if(inputBytesCounter<2000){//хотя функция QIODevice::read() и не даст забить в массив более 2000 байт, заполнение может происходить за несколько раз
        if(inputBytesCounter<=1){
            return;
        }
        if(inputArray[0]==(quint8)inputBytesCounter){//сравниваем размер пакета с его заголовком
            waitTimer->stop();//если пакет полностью получен, то сбрасываем таймер ожидания
            unsigned char crc=CRC16(inputArray,inputBytesCounter-1);//считаем CRC для всего пакета, кроме последнего байта
            if(crc==(unsigned char)inputArray[inputBytesCounter-1]){
                if(inputArray[1]==currentObject->getAddress()){//если адрес ответа соответствует адресу запроса
                    switch(inputArray[2]){//читаем ответ объекта
                        case(ANSWER_OK):{//если все в порядке читаем пакет
                            readPacket();
                            currentRequest=REQUEST_CLEAR;//и приказываем устройству очистить память
                            requestCurrentObject();
                            break;
                        }
                        case(ANSWER_ERROR):{//если ошибка CRC, повторно отправляем запрос
                            currentRequest=REQUEST_GET_DATA;
                            reRequestCurrentObject();
                            requestTimer->start(REQUEST_TIME);//переходим к следующему устройству
                            break;
                        }
                        case(ANSWER_NO_DATA):{//если данных нет
                            requestTimer->start(REQUEST_TIME);//переходим к следующему устройству
                            break;
                        }
                        case(ANSWER_CLEARED):{//если память очищена
                            readPacket();
                            requestTimer->start(REQUEST_TIME);//переходим к следующему устройству
                            break;
                        }
                        case(ANSWER_PORTS_STATE):{
                            currentRequest=REQUEST_GET_DATA;
                            requestCurrentObject();
                            requestTimer->start(REQUEST_TIME);
                            break;
                        }
                    }
                }
                else{
                    emit consoleMessage(tr("Вместо объекта ")+currentObject->getName()+tr(" с адресом")+QString::number(currentObject->getAddress())+
                                        tr(" ответил объект с адресом ")+QString::number(inputArray[1]));
                    currentObject->setOnline(false);
                    reRequestCurrentObject();
                }
            }
            else{//если СRC Не соответствуют
                reRequestCurrentObject();//повторный запрос
            }
        }
    }
    else{//если данных больше, чем 2000 байт, то это ошибка
        reRequestCurrentObject();//повторяем запрос
    }
    currentObject->setOnline(true);
}
///////////////////////////////////////////////////////////////////////////
void requestCore::port2DataReadyRead(){

}
//////////////////////////////////////////////////////////////////////////////////////
void requestCore::port1ErrorSlot(QSerialPort::SerialPortError error){
    switch(error){
        case(QSerialPort::NoError):{
            break;
        }
        case(QSerialPort::ResourceError):{//если порт физически удален из системы
            requestTimer->stop();
            waitTimer->stop();
            inputBytesCounter=0;
            currentPort->close();
            emit consoleMessage(tr("Последовательный порт ")+currentPort->portName()+tr(" отключен."));
            delete sPort1;
            sPort1=NULL;
            currentPort=NULL;
            requestTimer->start(REQUEST_TIME);
            break;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////
void requestCore::port2ErrorSlot(QSerialPort::SerialPortError error){
    switch(error){
        case(QSerialPort::NoError):{
            break;
        }
        case(QSerialPort::ResourceError):{//если порт физически удален из системы
            requestTimer->stop();
            waitTimer->stop();
            inputBytesCounter=0;
            currentPort->close();
            emit consoleMessage(tr("Последовательный порт ")+currentPort->portName()+tr(" отключен."));
            delete sPort2;
            sPort2=NULL;
            currentPort=NULL;
            requestTimer->start(REQUEST_TIME);
            break;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////
unsigned char *requestCore::generateArray(){
   quint16 size=10;
   unsigned char *array = new unsigned char[size];
   array[1]=(unsigned char)size;
   array[0]=(unsigned char)size>>8;
}
/////////////////////////////////////////////////////////////////////////////////////
void requestCore::waitTimeMashine(){
    mashine *tmpMashine=static_cast<mashine*>(currentObject);
    switch(currentRequest){//смотрим, после какого запроса сюда попали
        case(REQUEST_CLEAR):{//если запрашивали стирание памяти, то
            tmpMashine->memoryNotCleared();
            break;
        }
        case(REQUEST_GET_DATA):{//если запрашивали получение данных
            minutePoint point;
            point.event=EVENT_NO_RESPONCE;
            point.value=0;
            point.objectState=OBJECT_STATE_OFF;
            tmpMashine->setCurrentState(OBJECT_STATE_OFF);
            tmpMashine->addTimePoint(point,QTime::currentTime());
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////
