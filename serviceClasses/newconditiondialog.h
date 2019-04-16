#ifndef NEWCONDITIONDIALOG_H
#define NEWCONDITIONDIALOG_H

#include <QDialog>

namespace Ui {
class newConditionDialog;
}

class newConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newConditionDialog(QWidget *parent = 0);
    ~newConditionDialog();

private:
    Ui::newConditionDialog *ui;
};

#endif // NEWCONDITIONDIALOG_H
