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
    explicit newConditionDialog(objectPort *port, QWidget *parent = 0);
    ~newConditionDialog();
    void setPortName(QString name);
    void setStatisticCorePointer(statisticCore *pointer);

protected:
    objectPort *currentPort;
    statisticCore *statCorePointer;

protected slots:
    void portCheckSlot(int state);
    void stateCheckSlot(int state);
    void selectObject(int index);


private:
    Ui::newConditionDialog *ui;

};

#endif // NEWCONDITIONDIALOG_H
