#ifndef PORTTABLEWIDGET_H
#define PORTTABLEWIDGET_H

#include "widgets/logictablewidget.h"
#include "object.h"

class portTableWidget : public logicTableWidget
{
public:
    portTableWidget(QWidget *parent);
    virtual void updateContent();
    void setObject(object *newObject);

protected:
    object *currentObject;

signals:
    void selectSignal();


protected slots:
    void itemSelectSlot(int row, int column);
};

#endif // PORTTABLEWIDGET_H
