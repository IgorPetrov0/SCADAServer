#include "objectport.h"

objectPort::objectPort()
{
    type=PORT_OUTPUT;
    state=false;
    number=0;
}
////////////////////////////////////////////////////////////
objectPort::~objectPort(){
    int size=onConditions.size();
    for(int n=0;n!=size;n++){
        delete onConditions.at(n);
    }
    size=offConditions.size();
    for(int n=0;n!=size;n++){
        delete offConditions.at(n);
    }
}
///////////////////////////////////////////////////////////
int objectPort::getNumber() const{
    return number;
}
////////////////////////////////////////////////////////////////
void objectPort::setNumber(int value){
    number=value;
}
////////////////////////////////////////////////////////////////
QString objectPort::getDescription() const{
    return description;
}
/////////////////////////////////////////////////////////////////
void objectPort::setDescription(const QString &value){
    description=value;
}
///////////////////////////////////////////////////////////////////
QString objectPort::getName() const{
    return name;
}
//////////////////////////////////////////////////////////////////
void objectPort::setName(const QString &value){
    name=value;
}
///////////////////////////////////////////////////////////////////
void objectPort::serialisation(QDataStream *str){
    *str<<number;
    *str<<(int)type;
    *str<<name;
    *str<<description;
    int size=onConditions.size();
    *str<<size;
    for(int n=0;n!=size;n++){
        onConditions.at(n)->serialisation(str);
    }
    size=offConditions.size();
    *str<<size;
    for(int n=0;n!=size;n++){
        offConditions.at(n)->serialisation(str);
    }
}
/////////////////////////////////////////////////////////////////////
void objectPort::deserialisation(QDataStream *str){
    *str>>number;
    int tmp;
    *str>>tmp;
    type=(portTypes)tmp;
    *str>>name;
    *str>>description;
    int size=0;
    *str>>size;
    for(int n=0;n!=size;n++){
        condition *tmpCond = new condition;
        tmpCond->deserialisation(str);
        onConditions.append(tmpCond);
    }
    *str>>size;
    for(int n=0;n!=size;n++){
        condition *tmpCond = new condition;
        tmpCond->deserialisation(str);
        offConditions.append(tmpCond);
    }
}
////////////////////////////////////////////////////////////////////
int objectPort::getOnConditionsCount() const{
    return onConditions.size();
}
///////////////////////////////////////////////////////////////////
int objectPort::getOffConditionsCount(){
    return offConditions.size();
}
//////////////////////////////////////////////////////////////////
condition *objectPort::getOnCondition(int index){
    if((index>=0)&&(index<onConditions.size())){
        return onConditions.at(index);
    }
    return nullptr;
}
/////////////////////////////////////////////////////////////////////
condition *objectPort::getOffCondition(int index){
    if((index>=0)&&(index<offConditions.size())){
        return offConditions.at(index);
    }
    return nullptr;
}
///////////////////////////////////////////////////////////////
portTypes objectPort::getType() const{
    return type;
}
//////////////////////////////////////////////////////////////////
QString objectPort::getTypeString(){
    switch(type){
        case(PORT_INPUT):{
            return QObject::tr("Вход");
        }
        case(PORT_OUTPUT):{
            return QObject::tr("Выход");
        }
    }
}
//////////////////////////////////////////////////////////////////
void objectPort::setType(const portTypes &value){
    type = value;
}
/////////////////////////////////////////////////////////////////
objectPort &objectPort::operator=(const objectPort *right){
    if(right==this){
        return *this;
    }
    number=right->getNumber();
    name=right->getName();
    description=right->getDescription();
    type=right->getType();
    int size=right->getOnConditionsCount();
    for(int n=0;n!=size;n++){
        condition *tmpCondition = new condition;

    }



}
///////////////////////////////////////////////////////////
bool objectPort::getState() const{
    return state;
}
//////////////////////////////////////////////////////////////
void objectPort::setState(bool value){
    state = value;
}
//////////////////////////////////////////////////////////////////
void objectPort::addOnCondition(condition *onCondition){
    onConditions.append(onCondition);
}
////////////////////////////////////////////////////////////////////
void objectPort::addOffCondition(condition *offCondition){
    offConditions.append(offCondition);
}
