#ifndef NEWCONDITIONDIALOG_H
#define NEWCONDITIONDIALOG_H

#include <QDialog>
#include "defines.h"
#include "logic/objectport.h"
#include "statisticcore.h"

namespace Ui {
class newConditionDialog;
}

class newConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newConditionDialog(objectPort *port, QWidget *parent, int condIndex=-1);
    ~newConditionDialog();
    void setPortName(QString name);
    void setStatisticCorePointer(statisticCore *pointer);
    condition *getNewCondition();

protected:
    objectPort *currentPort;
    statisticCore *statCorePointer;
    condition *currentCondition;

protected slots:
    void portCheckSlot(int state);
    void stateCheckSlot(int state);
    void selectObject(int index);
    void okSlot();


private:
    Ui::newConditionDialog *ui;

};

#endif // NEWCONDITIONDIALOG_H
