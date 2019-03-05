#include "basewidget.h"

baseWidget::baseWidget()
{
    statCorePointer=NULL;
}
///////////////////////////////////////////////////////////////////
void baseWidget::setStatCorePointer(statisticCore *pointer){
    statCorePointer=pointer;
}
///////////////////////////////////////////////////////////////////
void baseWidget::updateContent(){

}
