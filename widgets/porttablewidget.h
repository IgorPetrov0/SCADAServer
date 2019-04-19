#ifndef PORTTABLEWIDGET_H
#define PORTTABLEWIDGET_H

#include "widgets/logictablewidget.h"
#include "object.h"

class portTableWidget : public logicTableWidget
{
    Q_OBJECT

public:
    portTableWidget(QWidget *parent);
    virtual void updateContent();
    void setObject(object *newObject);

protected:
    object *currentObject;

signals:
    void selectSignal(int index);


protected slots:
    void itemSelectSlot(int row, int column, int prevRow, int prevCol);
};

#endif // PORTTABLEWIDGET_H
