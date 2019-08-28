#include "condition.h"
#include "object.h"
#include "objectport.h"
#include "statisticcore.h"

condition::condition()
{
    targetObjectState=OBJECT_STATE_ANY;
    portState=false;
    time=0;
    targetObject=nullptr;
    targetPort=nullptr;
}
//////////////////////////////////////////////////////////////////////
QString condition::getDescription() const{
    return description;
}
//////////////////////////////////////////////////////////////////////
void condition::setDescription(const QString &value){
    description = value;
}
//////////////////////////////////////////////////////////////////////
QString condition::getTargetObjectName() const{
    if(targetObjectName.isEmpty()){
        return targetObject->getName();
    }
    return targetObjectName;
}
//////////////////////////////////////////////////////////////////////
void condition::setTargetObjectName(QString name){
    targetObjectName=name;
}
//////////////////////////////////////////////////////////////////////
int condition::getTargetPortNumber() const{
    return targetPortNumber;
}
//////////////////////////////////////////////////////////////////////
void condition::setTargetPortNumber(int number){
   targetPortNumber=number;
}
//////////////////////////////////////////////////////////////////////
bool condition::getPortState() const{
    return portState;
}
//////////////////////////////////////////////////////////////////////
QString condition::getPortStateString(){
    if(portState){
        return QObject::tr("Включен");
    }
    else{
        return QObject::tr("Отключен");
    }
}
//////////////////////////////////////////////////////////////////////
void condition::setPortState(bool value){
    portState = value;
}
//////////////////////////////////////////////////////////////////////
int condition::getTime() const{
    return time;
}
//////////////////////////////////////////////////////////////////////
void condition::setTime(const int value){
    time = value;
}
//////////////////////////////////////////////////////////////////////
logicType condition::getLogic() const{
    return logic;
}
///////////////////////////////////////////////////////////////////////
QString condition::getLogicString(){
    switch(logic){
        case(LOGIC_AND):{
            return QObject::tr("И");
        }
        case(LOGIC_NO):{
            return QObject::tr("НЕ");
        }
        case(LOGIC_OR):{
            return QObject::tr("ИЛИ");
        }
    }
}
//////////////////////////////////////////////////////////////////////
void condition::setLogic(const logicType &value){
    logic = value;
}
///////////////////////////////////////////////////////////////////////
objectState condition::getTargetObjectState() const{
    return targetObjectState;
}
/////////////////////////////////////////////////////////////////////////
QString condition::getTargetObjectStateString(){
    switch (targetObjectState) {
        case(OBJECT_STATE_ON):{
            return QObject::tr("Включен");
        }
        case(OBJECT_STATE_OFF):{
            return QObject::tr("Выключен");
        }
        case(OBJECT_STATE_WORK):{
            return QObject::tr("Работает");
        }
        case(OBJECT_STATE_ANY):{
            return QObject::tr("Любое");
        }
    }
}
/////////////////////////////////////////////////////////////////////////
void condition::setTargetObjectState(const objectState &value){
    targetObjectState = value;
}
////////////////////////////////////////////////////////////////////////
void condition::serialisation(QDataStream *str){
    *str<<description;
    *str<<(int)logic;
    *str<<targetObject->getName();
    *str<<(int)targetObjectState;
    *str<<targetPortNumber;
    *str<<(bool)portState;
    *str<<time;
}
////////////////////////////////////////////////////////////////////////
bool condition::deserialisation(QDataStream *str){
    *str>>description;
    if(description.size()>MAX_OBJECT_DESCRIPTION_SYMBOLS){
        return false;
    }
    int tmp=0;
    *str>>tmp;
    if((tmp<LOGIC_OR)||tmp>LOGIC_NO){
        return false;
    }
    logic=(logicType)tmp;
    *str>>targetObjectName;
    if(targetObjectName.isEmpty()||(targetObjectName.size()>MAX_OBJECT_NAME_SIZE)){
        return false;
    }
    *str>>tmp;
    if((tmp<OBJECT_STATE_ON)||(tmp>OBJECT_STATE_ANY)){
        return false;
    }
    targetObjectState=(objectState)tmp;
    *str>>targetPortNumber;
    if((targetPortNumber<0)||(targetPortNumber>MAX_OBJECT_PORTS)){
        return false;
    }
    *str>>portState;
    *str>>time;
    return true;
}
////////////////////////////////////////////////////////////////////////////
condition &condition::operator=(const condition *right){
    if(right==this){
        return *this;
    }
    description=right->getDescription();
    logic=right->getLogic();
    targetObjectName=right->getTargetObjectName();
    targetObjectState=right->getTargetObjectState();
    targetPortNumber=right->getTargetPortNumber();
    portState=right->getPortState();
    time=right->getTime();
    return *this;
}
//////////////////////////////////////////////////////////////////////////////
object *condition::getTargetObject() const{
    return targetObject;
}
<<<<<<< HEAD
////////////////////////////////////////////////////////////////////////////////
objectPort *condition::getTargetPort() const{
    return targetPort;
}
///////////////////////////////////////////////////////////////////////////////////
bool condition::generateTargetPointers(statisticCore *statCorePointer){
    if(statCorePointer!=nullptr){
        targetObject=statCorePointer->getObjectForName(targetObjectName);
        if(targetObject==nullptr){
            qDebug("condition::generateTargetPointers targetObject not found. The configuration file may not be fully read.");
            return false;
        }
        targetPort=targetObject->getPortByNumber(targetPortNumber);
        if(targetPort==nullptr){
            qDebug("condition::generateTargetPointers targetPort not found. The configuration file may not be fully read.");
            return false;
        }
        targetObjectName.clear();
        return true;
    }
    return false;
=======
////////////////////////////////////////////////////////////////////////////////////////
bool condition::findObjectPort(statisticCore *statCorePointer){
    targetObject=statCorePointer->getObjectForName(targetObjectName);
    if(targetObject==nullptr){
        return false;
    }
    targetObjectName.clear();
    targetPort=targetObject->getPortByNumber(targetPortNumber);
    if(targetPort==nullptr){
        return false;
    }
    return true;
>>>>>>> 9ee40cecae714103b6e03b277bd65aa2a1014df2
}
//////////////////////////////////////////////////////////////////////////////
