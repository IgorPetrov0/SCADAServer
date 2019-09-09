#include "tablepushbutton.h"

tablePushButton::tablePushButton(int index, QWidget *parent):
    QPushButton(parent)
{
    this->index=index;
    connect(this,SIGNAL(clicked()),this,SLOT(pushSlot()));
}
/////////////////////////////////////////////////////////////////////
void tablePushButton::pushSlot(){
    if(this->isChecked()){
        emit pushSignal(true,index);
    }
    else{
        emit pushSignal(false,index);
    }

}
