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
    explicit newConditionDialog(statisticCore *pointer, objectPort *port, QWidget *parent, bool on, int condIndex=-1);
    ~newConditionDialog();
    void setPortNumber(int number);
    condition *getNewCondition();



protected:
    object *currentObject;
    objectPort *currentPort;
    statisticCore *statCorePointer;
    condition *currentCondition;
    bool editMode;
    void viewCurrentCondition();

protected slots:
    void portCheckSlot(int state);
    void stateCheckSlot(int state);
    void selectObject(int index);
    void selectPortSlot(int index);
    void okSlot();


private:
    Ui::newConditionDialog *ui;

};

#endif // NEWCONDITIONDIALOG_H
