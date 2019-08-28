#include "statisticcore.h"

statisticCore::statisticCore(QObject *parent):
    QObject(parent)
{
    QTime time;
    qsrand(time.secsTo(QTime::currentTime()));
}
/////////////////////////////////////////////////////////////////////////////
statisticCore::~statisticCore()
{
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        delete mashinesArray.at(n);
    }
    mashinesArray.clear();
}
/////////////////////////////////////////////////////////////////////////////////
bool statisticCore::readConfiguration(QString workingDir){
    QFile file(workingDir+"/"+CONFIG_FILE_NAME);
    if(!file.open(QIODevice::ReadOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации."));
        return false;
    }
    QDataStream str(&file);
    str.setVersion(DATA_STREAM_VERSION);
    QString sig;
    str>>sig;
    if(sig!=SIGNATURE){
        setLastError(tr("Ошибка чтения сигнатуры конфигурационного файла"));
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
    int mashinesCount;//todo сначала машины, потом добавить другие объекты
    str>>mashinesCount;
    bool ok=true;
    for(int n=0;n!=mashinesCount;n++){
        if(!createObject(&str)){
            ok=false;
        }
    }
    if(!ok){//если была хоть одна ошибка
        file.close();
        return false;//то выходим с ошибкой(ошибка прописалась в функции createObject)
    }
    file.close();
    emit consoleMessage(tr("Конфигурация статистики прочитана из файла ")+workingDir+"/"+CONFIG_FILE_NAME);    

    //проверяем наличие сегодняшних файлов и если есть загружаем
    int size=mashinesArray.size();
    QString currentDate=QDate::currentDate().toString("_dd_MM_yyyy")+".stat";
    for(int n=0;n!=size;n++){
        mashine *tmpMashine=mashinesArray.at(n);
        QString fileName=tmpMashine->getPathForStatistics()+"/"+tmpMashine->getName()+currentDate;
        file.setFileName(fileName);
        if(file.exists()){
            dayGraph *todayGraph=readGraphFile(fileName);
            if(todayGraph!=nullptr){
                tmpMashine->setCurrentGraph(todayGraph);
            }
            else{
                return false;
            }
        }
    }
    //генерируем указатели на объекты у всех состояний всех портов
    if(!generatePointersForConditions()){
        setLastError(tr("Ошибка чтения логики управления."));
        return false;
    }

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::writeConfiguration(QString workingDir){
    QFile file(workingDir+"/"+CONFIG_FILE_NAME);
    if(!file.open(QIODevice::WriteOnly)){
        setLastError(tr("Невозможно открыть файл конфигурации."));
        return false;
    }
    QDataStream str(&file);
    str.setVersion(DATA_STREAM_VERSION);
    str<<QString(SIGNATURE);
    str<<(float)CUR_VERSION;
    int objectsCount=mashinesArray.size();
    str<<objectsCount;
    for(int n=0;n!=objectsCount;n++){
        mashinesArray.at(n)->serialisation(&str);
    }

    file.close();
    emit consoleMessage(tr("Конфигурация статистики записана в файл ")+workingDir+"/"+CONFIG_FILE_NAME);
    return true;
}
////////////////////////////////////////////////////////////////////////////
int statisticCore::getObjectsCount(){
    int count=0;
    count+=mashinesArray.size();//todo Общее число объектов состоит из машин и чего-то еще, что придумаем позже

    return count;
}
///////////////////////////////////////////////////////////////////////////////////
object *statisticCore::getObjectForIndex(int index){
    if((index>=0)&&(index<mashinesArray.size())){
        return mashinesArray.at(index);
    }
    //todo в дальнейшем проверяем на mashinesArray.size+anythingElse.size
    return nullptr;
}
///////////////////////////////////////////////////////////////////////////////////////////
object *statisticCore::getObjectForName(QString name){
    int size=mashinesArray.size();//проверяем по массиву машин
    for(int n=0;n!=size;n++){
        object *tmp=mashinesArray.at(n);
        if(mashinesArray.at(n)->getName()==name){
            return tmp;
        }
    }
    //todo проверяем по остальным массивам объектов

    return nullptr;
}
/////////////////////////////////////////////////////////////////////////////////////
int statisticCore::getObjectIndex(object *targetObject){
    if(targetObject==nullptr){
        return -1;
    }

    int size=mashinesArray.size();//сначала проверяем массив машин
    int counter=0;
    for(int n=0;n!=size;n++){
        if(mashinesArray.at(n)==targetObject){
            return counter;
        }
        counter++;
    }
    //todo затем проверяем другие массивы инкрементируя counter

    return -1;
}
/////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::createObject(QDataStream *str, bool remout){
    object tmpObject;
    bool ok=true;
    if(!tmpObject.deserialisation(str)){//object десериализовали здесь. Дальше десериализуем потомков
        setLastError(tr("Ошибка чтения объекта. Файл конфигурации поврежден."));
        return false;
    }
    switch(tmpObject.getType()){
        case(objectMashine):{
            mashine *tmpMashine = new mashine;
            *tmpMashine=tmpObject;
            tmpMashine->deserialisationContinue(str);//здесь десериализуем только mashine
            if(remout){//если машина создается удаленно, то прописываем локальный путь для статистики
                tmpMashine->setPathForStatistics(QApplication::applicationDirPath()+STAT_PATH);
            }
            QDir dir(tmpMashine->getPathForStatistics());
            if(!dir.exists()){//если папка машины не существует, то создаем
                QString path=dir.absolutePath();
                if(!dir.mkdir(path)){
                    QString error=tr("Не удалось создать папку для машины ")+
                            tmpMashine->getName()+tr("\n по адресу ")+
                            QApplication::applicationDirPath()+STAT_PATH;

                    setLastError(error);
                    emit consoleMessage(error);
                    ok=false;//ошибка не критичная. машина создается, даже если не удалось создать папку
                }
            }
            mashinesArray.append(tmpMashine);
            connect(tmpMashine,SIGNAL(newDaySygnal(mashine*)),this,SLOT(newDaySlot(mashine*)));
            //generateTestGraph(tmpMashine);//генерация файлов статистики для отладки
            break;
        }
        //todo
    }
    return ok;
}
////////////////////////////////////////////////////////////////////////////
int statisticCore::getMashinsCount(){
    return mashinesArray.size();
}
////////////////////////////////////////////////////////////////////////////
mashine *statisticCore::getMashine(int index){
    if((index>=mashinesArray.size())||(index<0)){
        qDebug("statisticCore::getMashine : index out of range");
        return nullptr;
    }
    return mashinesArray.at(index);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::isNameExist(QString name, object *ob){
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        object *tmp=mashinesArray.at(n);
        if(ob!=tmp){//не проверяем указанный объект
            if(mashinesArray.at(n)->getName()==name){
                return true;
            }
        }
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////
object *statisticCore::getObjectForAddress(int address, object *ob){
    int size=mashinesArray.size();
    object *tmp=nullptr;
    for(int n=0;n!=size;n++){
        tmp=mashinesArray.at(n);
        if(tmp!=ob){//не проверяем указанный объект. Во первых нет смысла, во вторых для возможности редактировать
            if(tmp->getAddress()==address){
                return tmp;
            }
        }
    }
    return nullptr;
}
//////////////////////////////////////////////////////////////////
bool statisticCore::mashineUp(int index){
    if(index>0){
        mashine *tmp=mashinesArray.at(index);
        mashinesArray.remove(index);
        mashinesArray.insert(index-1,tmp);
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////////////
bool statisticCore::mashineDown(int index){
    if(index<mashinesArray.size()-1){
        mashine *tmp=mashinesArray.at(index);
        mashinesArray.remove(index);
        mashinesArray.insert(index+1,tmp);
        return true;
    }
    return false;
}
///////////////////////////////////////////////////////////////////////////////////////////
reportClass *statisticCore::createReport(reportType type, QDate startDate, QDate stopDate, int mashineIndex){

    //получаем массив файлов статистики по указанным датам
    mashine *tmpPointer=mashinesArray.at(mashineIndex);
    QString path=tmpPointer->getPathForStatistics();
    qint64 count=startDate.daysTo(stopDate)+1;
    QVector<dayGraph*>dayGraphsArray;
    QVector<QString>filesNames;//имена файлов, участвующих в рассчетах
    for(qint64 n=0;n!=count;n++){
        qint64 julDay=startDate.toJulianDay();
        julDay+=n;
        QDate tmpDate=QDate::fromJulianDay(julDay);
        QString fileName=path+"/"+tmpPointer->getName()+"_"+tmpDate.toString("dd_MM_yyyy")+".stat";
        QFileInfo file(fileName);
        if(file.exists()){
            dayGraph *tmpDayGraph = readGraphFile(fileName);
            if(tmpDayGraph!=nullptr){
                dayGraphsArray.append(tmpDayGraph);
                filesNames.append(fileName);
            }
            else{//при неудачном открытии
                QString tmp=getLastError();//сбрасываем ошибку. Она нам не нужна. Просто не запоминаем график
            }
        }
    }
    if(filesNames.size()==0){
        setLastError(tr("Данные за период с ")+startDate.toString("dd_MM_yyyy")+
                     tr(" по ")+stopDate.toString("dd_MM_yyyy")+tr(" для машины ")+
                     tmpPointer->getName()+tr(" отсутствуют."));
        return nullptr;
    }

    switch(type){
        case(REPORT_DAY):{

            break;
        }
        case(REPORT_SHIFT):{
            shiftReport *report = new shiftReport;
            int shiftSize=tmpPointer->getShiftsCount();
            QVector<shift>shiftsArray;
            for(int n=0;n!=shiftSize;n++){
                shiftsArray.append(tmpPointer->getShift(n));
            }
            report->createReport(&dayGraphsArray,&shiftsArray,&filesNames);
            return report;
        }
        case(REPORT_ALL_MASHINES):{

            break;
        }
    }
    return nullptr;
}
///////////////////////////////////////////////////////////////////
void statisticCore::deleteObject(object *ob, bool objectOnly){
    switch(ob->getType()){
        case(objectMashine):{
            mashine *m=static_cast<mashine*>(ob);
            int size=mashinesArray.size();
            for(int n=0;n!=size;n++){
                mashine *tmp=mashinesArray.at(n);
                if(tmp==m){
                    if(!objectOnly){
                        removeDirRecursively(tmp->getPathForStatistics());
                    }
                    delete tmp;
                    mashinesArray.remove(n);
                    break;
                }
            }
            break;
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
dayGraph *statisticCore::readGraphFile(QString fullPath){
    QFile file(fullPath);
    if(file.open(QIODevice::ReadOnly)){
        dayGraph *dayStruct = new dayGraph;
        dayStruct->minutesArray=new minutePoint[1440];
        QDataStream str(&file);
        str>>dayStruct->name;
        QDate date;
        str>>date;
        dayStruct->date=date;
        for(int n=0;n!=1440;n++){
            //получаем минуту из массива
            minutePoint point;
            str>>point.value;
            str>>point.event;
            str>>point.objectState;
            dayStruct->minutesArray[n]=point;
            if(str.atEnd() && n!=1439){
                setLastError(tr("Файл ")+fullPath+tr(" поврежден. Данные не прочитаны."));
                delete[] dayStruct->minutesArray;
                delete dayStruct;
                return nullptr;
            }
        }
        return dayStruct;
    }
    setLastError(tr("Невозможно открыть файл \n")+fullPath);
    return nullptr;
}
///////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::writeGraphsInFiles(){
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        mashine *tmpMashine=mashinesArray.at(n);
        if(tmpMashine->isRequestEnable()){
            if(!writeGraph(tmpMashine)){
                setLastError(tr("Невозможно открыть файл ")+tmpMashine->getPathForStatistics()+"/"+tmpMashine->getName()+"_"+
                             QDate::currentDate().toString("dd_MM_yyyy")+".stat");
                return false;
            }
        }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
QStringList statisticCore::getObjectsNamesList(){
    QStringList list;
    int size=getObjectsCount();
    for(int n=0;n!=size;n++){
        list.append(getObjectForIndex(n)->getName());
    }
    return list;
}
///////////////////////////////////////////////////////////////////////////////////
void statisticCore::checkConditions(){
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        object *tmpObject=mashinesArray.at(n);
        int portsCount=tmpObject->getPortsCount();
        for(int m=0;m!=portsCount;m++){
            objectPort *tmpPort=tmpObject->getPort(n);
            if(tmpPort->getType()==PORT_OUTPUT){

            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////
bool statisticCore::checkANDConditions(objectPort *port, bool on_off){

}
////////////////////////////////////////////////////////////////////////////
bool statisticCore::checkORConditions(objectPort *port,bool on){

}
///////////////////////////////////////////////////////////////////////////
bool statisticCore::checkNOTConditions(objectPort *port, bool on_off){

}
/////////////////////////////////////////////////////////////////////////////
void statisticCore::clearAll(){
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        delete mashinesArray.at(n);
    }
    mashinesArray.clear();
}
////////////////////////////////////////////////////////////////////////////
void statisticCore::generateTestGraph(mashine *m){
    QFile file(m->getPathForStatistics()+"/"+m->getName()+"_"+
               QDate::currentDate().toString("dd_MM_yyyy")+".stat");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream str(&file);
        str<<m->getName();
        str<<QDate::currentDate();
        for(int n=0;n!=1440;n++){
            int r=qrand()%((1000+1)-600)+600;
            str<<r;
            str<<(int)EVENT_OK;
        }
        file.close();
    }
}
///////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::removeDirRecursively(QString dirPath){
    QDir dir(dirPath);
    if(!dir.exists()){
        setLastError(tr("Папка ")+dir.dirName()+tr(" не найдена"));
        return false;
    }

    QFileInfoList list=dir.entryInfoList(QDir::NoDotAndDotDot|QDir::Dirs|QDir::Files);
    int count=list.size();
    for(int n=0;n!=count;n++){
        QFileInfo info=list.at(n);
        if(info.isDir()){
            if(!removeDirRecursively(info.absoluteFilePath())){
                return false;
            }
        }
        else{
            if(!dir.remove(info.fileName())){
                setLastError(tr("Невозможно удалить файл ")+info.fileName()+tr("\n Прервано."));
                return false;
            }
        }
    }
    dir.rmdir(dirPath);
    return true;
}
////////////////////////////////////////////////////////////////////////////////
bool statisticCore::writeGraph(mashine *tmpMashine){
    QFile file(tmpMashine->getPathForStatistics()+"/"+tmpMashine->getName()+"_"+
                     QDate::currentDate().toString("dd_MM_yyyy")+".stat");
    if(file.open(QIODevice::WriteOnly)){
        QDataStream str(&file);
        tmpMashine->serialiseDayGraph(&str);
        file.close();
        return true;
    }
    else{
        return false;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::generatePointersForConditions(){
    int size=mashinesArray.size();//todo проходим по массиву машин. Потом добавить другие массивы
    for(int n=0;n!=size;n++){
        object *tmpObject=mashinesArray.at(n);
        int portsCount=tmpObject->getPortsCount();
        for(int m=0;m!=portsCount;m++){
            objectPort *tmpPort=tmpObject->getPort(m);
            int onCount=tmpPort->getOnConditionsCount();
            int offCount=tmpPort->getOffConditionsCount();
            for(int onN=0;onN!=onCount;onN++){//проходим по состояниям включения
                if(!tmpPort->getOnCondition(onN)->generateTargetPointers(this)){//если споткнулись - дальше продолжать нет смысла. база повреждена
                    return false;
                }
            }
            for(int offN=0;offN!=offCount;offN++){//проходим по состояниям выключения
                if(!tmpPort->getOnCondition(offN)->generateTargetPointers(this)){//если споткнулись - дальше продолжать нет смысла. база повреждена
                    return false;
                }
            }
        }
    }
    return true;
}
///////////////////////////////////////////////////////////////////////////////////
void statisticCore::newDaySlot(mashine *tmpMashine){
    if(!writeGraph(tmpMashine)){
        emit consoleMessage(tr("Не удалось сохранить график за ")+QDate::currentDate().toString("dd_MM_yyyy")+
                            tr(" для машины <")+tmpMashine->getName()+">");
    }
}
//////////////////////////////////////////////////////////////////////////////
