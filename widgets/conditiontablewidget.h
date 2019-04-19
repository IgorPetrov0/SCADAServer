#ifndef CONDITIONTABLEWIDGET_H
#define CONDITIONTABLEWIDGET_H

#include <QWidget>
#include "widgets/logictablewidget.h"
#include "object.h"

namespace Ui {
class logicTableWidget;
}



class conditionTableWidget : public logicTableWidget
{
    Q_OBJECT

public:
    conditionTableWidget(QWidget *parent);
    virtual void updateContent();

protected:
    void fillTableString(condition *tmpCond, int row);

};

#endif // CONDITIONTABLEWIDGET_H
