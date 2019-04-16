#include "newconditiondialog.h"
#include "ui_newconditiondialog.h"

newConditionDialog::newConditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newConditionDialog)
{
    ui->setupUi(this);
}

newConditionDialog::~newConditionDialog()
{
    delete ui;
}
