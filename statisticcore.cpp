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
    int mashinesCount;
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
            if(todayGraph!=NULL){
                tmpMashine->setCurrentGraph(todayGraph);
            }
            else{
                return false;
            }
        }
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
    count+=mashinesArray.size();//Общее число объектов состоит из машин и чего-то еще, что придумаем позже

    return count;
}
///////////////////////////////////////////////////////////////////////////////////
object *statisticCore::getObjectForIndex(int index){
    if(index<mashinesArray.size()){
        return mashinesArray.at(index);
    }
    //в дальнейшем проверяем на mashinesArray.size+anythingElse.size
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
bool statisticCore::createObject(QDataStream *str, bool remout){
    object tmpObject;
    bool ok=true;
    tmpObject.deserialisation(str);//object десериализовали здесь. Дальше десериализуем потомков
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
                            QApplication::applicationDirPath()+"/"+STAT_PATH;

                    setLastError(error);
                    emit consoleMessage(error);
                    ok=false;//ошибка не критичная. машина создается, даже если не удалось создать папку
                }
            }
            mashinesArray.append(tmpMashine);
            //generateTestGraph(tmpMashine);//генерация файлов статистики для отладки
            break;
        }
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
            if(tmpDayGraph!=NULL){
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
        return NULL;
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
    return NULL;
}
///////////////////////////////////////////////////////////////////
bool statisticCore::deleteObject(object *ob, bool objectOnly){
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
            dayStruct->minutesArray[n]=point;
            if(str.atEnd() && n!=1439){
                setLastError(tr("Файл ")+fullPath+tr(" поврежден. Данные не прочитаны."));
                delete[] dayStruct->minutesArray;
                delete dayStruct;
                return NULL;
            }
        }
        return dayStruct;
    }
    setLastError(tr("Невозможно открыть файл \n")+fullPath);
    return NULL;
}
///////////////////////////////////////////////////////////////////////////////////////
bool statisticCore::writeGraphsInFiles(){
    int size=mashinesArray.size();
    for(int n=0;n!=size;n++){
        mashine *tmpMashine=mashinesArray.at(n);
        if(tmpMashine->isRequestEnable()){
            QFile file(tmpMashine->getPathForStatistics()+"/"+tmpMashine->getName()+"_"+
                             QDate::currentDate().toString("dd_MM_yyyy")+".stat");
            if(file.open(QIODevice::WriteOnly)){
                QDataStream str(&file);
                tmpMashine->serialiseDayGraph(&str);
                file.close();
            }
            else{
                setLastError(tr("Невозможно открыть файл ")+file.fileName());
                return false;
            }
        }
    }
    return true;
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
//////////////////////////////////////////////////////////////////////////////
