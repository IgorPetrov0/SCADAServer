#include "newportdialog.h"
#include "ui_newportdialog.h"

newPortDialog::newPortDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newPortDialog)
{
    ui->setupUi(this);
}

newPortDialog::~newPortDialog()
{
    delete ui;
}
