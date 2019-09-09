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
void objectPort::serialisation(QDataStream *str){
    *str<<number;
    *str<<(int)type;
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
bool objectPort::deserialisation(QDataStream *str){
    *str>>number;
    if((number<0)||(number>MAX_OBJECT_PORTS)){
        return false;
    }
    int tmp;
    *str>>tmp;
    type=(portTypes)tmp;
    if((type<PORT_INPUT)||(type>PORT_OUTPUT)){
        return false;
    }
    *str>>description;
    if(description.size()>MAX_OBJECT_DESCRIPTION_SYMBOLS){
        return false;
    }
    int size=0;
    *str>>size;
    if((size<0)||(size>MAX_CONDITIONS)){
        return false;
    }
    for(int n=0;n!=size;n++){
        condition *tmpCond = new condition;
        tmpCond->deserialisation(str);
        onConditions.append(tmpCond);
    }
    *str>>size;
    if((size<0)||(size>MAX_CONDITIONS)){
        return false;
    }
    for(int n=0;n!=size;n++){
        condition *tmpCond = new condition;
        tmpCond->deserialisation(str);
        offConditions.append(tmpCond);
    }
    return true;
}
////////////////////////////////////////////////////////////////////
int objectPort::getOnConditionsCount() const{
    return onConditions.size();
}
///////////////////////////////////////////////////////////////////
int objectPort::getOffConditionsCount() const{
    return offConditions.size();
}
//////////////////////////////////////////////////////////////////
condition *objectPort::getOnCondition(int index) const{
    if((index>=0)&&(index<onConditions.size())){
        return onConditions.at(index);
    }
    return nullptr;
}
/////////////////////////////////////////////////////////////////////
condition *objectPort::getOffCondition(int index) const{
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
objectPort &objectPort::operator=(const objectPort &right){
    if(&right==this){
        return *this;
    }
    number=right.getNumber();
    description=right.getDescription();
    type=right.getType();
    int size=right.getOnConditionsCount();
    for(int n=0;n!=size;n++){
        condition *tmpCondition = new condition;
        *tmpCondition=*right.getOnCondition(n);
        onConditions.append(tmpCondition);
    }
    size=right.getOffConditionsCount();
    for(int n=0;n!=size;n++){
        condition *tmpCondition = new condition;
        *tmpCondition=*right.getOffCondition(n);
        offConditions.append(tmpCondition);
    }
    return *this;
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
//////////////////////////////////////////////////////////////////////////
void objectPort::deleteOnCondition(int index){
    delete onConditions.at(index);
    onConditions.remove(index);
}
//////////////////////////////////////////////////////////////////////////
void objectPort::deleteOffCondition(int index){
    delete offConditions.at(index);
    offConditions.remove(index);
}
/////////////////////////////////////////////////////////////////////////
void objectPort::checkOnConditions(){
    QVector<condition*> andConditions;
    QVector<condition*> orConditions;
    QVector<condition*> noConditions;

    int size=onConditions.size();
    if(size!=0){
        for(int n=0;n!=size;n++){
            condition *tmpCondition=onConditions.at(n);
            if(tmpCondition->getLogic()==LOGIC_NO){
                noConditions.append(tmpCondition);
            }
            else if(tmpCondition->getLogic()==LOGIC_OR){
                orConditions.append(tmpCondition);
            }
            else if(tmpCondition->getLogic()==LOGIC_AND){
                andConditions.append(tmpCondition);
            }
        }

        size=orConditions.size();
        if(size!=0){
            for(int n=0;n!=size;n++){
                if(orConditions.at(n)->checkCondition()){//если хотябы одно условие соответствует
                    this->setState(true);//то включаем порт
                    return;
                }
            }
        }

        size=andConditions.size();
        if(size!=0){
            int n=0;
            for(;n!=size;n++){
                if(!andConditions.at(n)->checkCondition()){//если хотябы одно условие не соответствует
                    break;//останавливаемся
                }
            }
            if(n==size){//если прошли все условия и все соответствуют
                this->setState(true);//то включаем порт
                return;
            }
        }

        size=noConditions.size();
        if(size!=0){
            int n=0;
            for(;n!=size;n++){
                if(noConditions.at(n)->checkCondition()){
                    break;
                }
            }
            if(n==size){//если прошли все условия и все не соответствуют
                this->setState(true);//то включаем порт
                return;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
void objectPort::checkOffConditions(){
    QVector<condition*> andConditions;
    QVector<condition*> orConditions;
    QVector<condition*> noConditions;

    int size=onConditions.size();
    if(size!=0){
        for(int n=0;n!=size;n++){
            condition *tmpCondition=onConditions.at(n);
            if(tmpCondition->getLogic()==LOGIC_NO){
                noConditions.append(tmpCondition);
            }
            else if(tmpCondition->getLogic()==LOGIC_OR){
                orConditions.append(tmpCondition);
            }
            else if(tmpCondition->getLogic()==LOGIC_AND){
                andConditions.append(tmpCondition);
            }
        }
        size=orConditions.size();
        if(size!=0){
            for(int n=0;n!=size;n++){
                if(orConditions.at(n)->checkCondition()){//если хотябы одно условие соответствует
                    this->setState(false);//то выключаем порт
                    return;
                }
            }
        }

        size=andConditions.size();
        if(size!=0){
            int n=0;
            for(;n!=size;n++){
                if(!andConditions.at(n)->checkCondition()){//если хотябы одно условие не соответствует
                    break;//останавливаемся
                }
            }
            if(n==size){//если прошли все условия и все соответствуют
                this->setState(false);//то выключаем порт
                return;
            }
        }

        size=noConditions.size();
        if(size!=0){
            int n=0;
            for(;n!=size;n++){
                if(noConditions.at(n)->checkCondition()){
                    break;
                }
            }
            if(n==size){//если прошли все условия и все не соответствуют
                this->setState(false);//то выключаем порт
                return;
            }
        }
    }
}


