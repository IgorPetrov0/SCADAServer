#include "condition.h"
#include "object.h"
#include "objectport.h"

condition::condition()
{
    targetObjectState=OBJECT_STATE_ANY;
    portState=false;
    time=0;
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
    return targetObjectName;
}
//////////////////////////////////////////////////////////////////////
void condition::setTargetObjectName(QString name){
    targetObjectName=name;
}
//////////////////////////////////////////////////////////////////////
QString condition::getTargetPortName() const{
    return targetPortName;
}
//////////////////////////////////////////////////////////////////////
void condition::setTargetPortName(QString name){
   targetPortName=name;
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
    *str<<targetObjectName;
    *str<<(int)targetObjectState;
    *str<<targetPortName;
    *str<<(bool)portState;
    *str<<time;
}
////////////////////////////////////////////////////////////////////////
void condition::deserialisation(QDataStream *str){
    *str>>description;
    int tmp=0;
    *str>>tmp;
    logic=(logicType)tmp;
    *str>>targetObjectName;
    *str>>tmp;
    targetObjectState=(objectState)tmp;
    *str>>targetPortName;
    *str>>portState;
    *str>>time;
}
