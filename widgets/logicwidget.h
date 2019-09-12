#ifndef LOGICKWIDGET_H
#define LOGICKWIDGET_H

#include <QWidget>
#include "basewidget.h"
#include "widgets/logictablewidget.h"
#include "serviceClasses/newportdialog.h"
#include "serviceClasses/newconditiondialog.h"
#include "serviceClasses/portsmanualdialog.h"



namespace Ui {
class logicWidget;
}

class logicWidget : public QWidget, public baseWidget
{
    Q_OBJECT

public:
    explicit logicWidget(QWidget *parent = 0);
    ~logicWidget();
    virtual void updateContent();
    void clear();

protected:
    void resizeEvent(QResizeEvent *event);
    object *currentObjectPointer;
    objectPort *currentPortPointer;


protected slots:
    void selectObjectSlot(int index);
    void selectPortSlot(int index);
    void addSlot(tableType type);
    void editSlot(tableType type,int index);
    void deleteSlot(tableType type, int index);
    void maualManage();

private:
    Ui::logicWidget *ui;
};

#endif // LOGICKWIDGET_H
