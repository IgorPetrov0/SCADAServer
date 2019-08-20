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
    explicit logicTableWidget(QWidget *parent = nullptr);
    void setCurrentPort(objectPort *port);
    void setTableType(tableType type);
    virtual void updateContent();
    virtual void clear();
    objectPort *getCurrentPort();
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
