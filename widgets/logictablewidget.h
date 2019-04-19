#ifndef LOGICTABLEWIDGET_H
#define LOGICTABLEWIDGET_H

#include <QWidget>
#include "logic/objectport.h"

namespace Ui {
class logicTableWidget;
}

class logicTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit logicTableWidget(QWidget *parent = 0);
    void setCurrentPort(objectPort *port);
    virtual void updateContent();
    void setTableType(tableType type);
    ~logicTableWidget();


signals:
    void addSignal(tableType type);
    void editSignal(tableType type, int index);
    void deleteSignal(tableType type, int index);

protected slots:
    void addSlot();
    void editSlot();
    void deleteSlot();

protected:
    void resizeEvent(QResizeEvent *event);
    Ui::logicTableWidget *ui;
    tableType type;
    objectPort *currentPort;
};

#endif // LOGICTABLEWIDGET_H
