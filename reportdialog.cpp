#include "reportdialog.h"
#include "ui_reportdialog.h"

reportDialog::reportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportDialog)
{
    ui->setupUi(this);
}

reportDialog::~reportDialog()
{
    delete ui;
}
