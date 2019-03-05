#include "newobjectmasterpage.h"

newObjectMasterPage::newObjectMasterPage(QWidget *parent):
    QWidget(parent)
{
    currentObject=NULL;
    int stage=0;
    createMode=true;
}
//////////////////////////////////////////////////////////////////////////////
QByteArray newObjectMasterPage::getData(){
    return QByteArray();
}
//////////////////////////////////////////////////////////////////////////////////
int newObjectMasterPage::getStage() const{
    return stage;
}
/////////////////////////////////////////////////////////////////////////////////
void newObjectMasterPage::setObject(object **newObject){
    currentObject=*newObject;
}
///////////////////////////////////////////////////////////////////////////
void newObjectMasterPage::loadObject(object *existObject){
    currentObject=existObject;
}
/////////////////////////////////////////////////////////////////////////////
bool newObjectMasterPage::next(){
    return false;
}
