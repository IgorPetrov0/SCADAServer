#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include "statisticcore.h"

class baseWidget
{
public:
    baseWidget();
    virtual ~baseWidget();
    virtual void setStatCorePointer(statisticCore *pointer);
    virtual void updateContent();


protected:
    statisticCore *statCorePointer;

};

#endif // BASEWIDGET_H
