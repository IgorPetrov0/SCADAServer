#include "object.h"

object::object(QObject *parent) : QObject(parent)
{
    name.clear();
    address=0;
    description.clear();
    type=objectUnknow;
    requestEnable=true;
    online=false;
}
//////////////////////////////////////////////////////////
object::~object(){
    int size=ports.size();
    for(int n=0;n!=size;n++){
        delete ports.at(n);
    }
}
//////////////////////////////////////////////////////////
int object::getAddress() const{
    return address;
}
//////////////////////////////////////////////////////////
void object::setAddress(unsigned int value){
    address = value;
}
////////////////////////////////////////////////////////////
QString object::getName() const{
    return name;
}
//////////////////////////////////////////////////////////////
void object::setName(const QString &value){
    name = value;
}
///////////////////////////////////////////////////////////////////////
QString object::getDescription() const{
    return description;
}
////////////////////////////////////////////////////////////////////////
void object::setDescription(const QString &value){
    description = value;
}
///////////////////////////////////////////////////////////
void object::serialisation(QDataStream *str){
    *str<<name;
    *str<<(int)type;
    *str<<address;
    *str<<description;
    *str<<requestEnable;
    int size=ports.size();
    *str<<size;
    for(int n=0;n!=size;n++){
        ports.at(n)->serialisation(str);
    }
}
///////////////////////////////////////////////////////////////////
void object::deserialisation(QDataStream *str){
    *str>>name;
    int tmp;
    *str>>tmp;
    type=(objectType)tmp;
    *str>>address;
    *str>>description;
    *str>>requestEnable;
    int size;
    *str>>size;
    for(int n=0;n!=size;n++){
        objectPort *tmpPort = new objectPort();
        tmpPort->deserialisation(str);
        ports.append(tmpPort);
    }
}
////////////////////////////////////////////////////////////////////////
void object::deserialisationContinue(QDataStream *str){
    *str>>name;
    int tmp;
    *str>>tmp;
    type=(objectType)tmp;
    *str>>address;
    *str>>description;
    int size;
    *str>>size;
    for(int n=0;n!=size;n++){
        objectPort *tmpPort = new objectPort();
        tmpPort->deserialisation(str);
        ports.append(tmpPort);
    }
}
///////////////////////////////////////////////////////////////////////////
objectType object::getType() const{
    return type;
}
//////////////////////////////////////////////////////////////////////////
void object::setType(const objectType &value){
    type = value;
}
////////////////////////////////////////////////////////////////////////////
QString object::getTypeString(){
    switch(type){
        case(objectMashine):{
            return tr("Машина");
        }
        default:{
            return QString();
        }
    }
}
//////////////////////////////////////////////////////////////////
bool object::isOnline(){
    return online;
}
///////////////////////////////////////////////////////////////////
void object::setOnline(bool value){
    online=value;
}
////////////////////////////////////////////////////////////////////
bool object::isRequestEnable(){
    return requestEnable;
}
///////////////////////////////////////////////////////////////////
void object::setRequestEnable(bool enable){
    requestEnable=enable;
}
///////////////////////////////////////////////////////////////////
int object::getPortsCount() const{
    return ports.size();
}
//////////////////////////////////////////////////////////////////
objectPort *object::getPort(int index) const{
    if((index>=0)&&(index<ports.size())){
        return ports.at(index);
    }
    return nullptr;
}
/////////////////////////////////////////////////////////////////
void object::addPort(objectPort *port){
    if(port!=nullptr){
        ports.append(port);
    }
    else{
        qDebug("object::addPort port is NULL");
    }
}
///////////////////////////////////////////////////////////
object& object::operator=(const object& right){
    if (this == &right) {
        return *this;
    }
    address=right.getAddress();
    name=right.getName();
    description=right.getDescription();
    type=right.getType();
    int size=right.getPortsCount();
    for(int n=0;n!=size;n++){
        objectPort *tmpPort = new objectPort;
        tmpPort=right.getPort(n);
        ports.append(tmpPort);
    }
    return *this;
}
