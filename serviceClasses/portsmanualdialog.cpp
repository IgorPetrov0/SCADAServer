#include "portsmanualdialog.h"
#include "ui_portsmanualdialog.h"

portsManualDialog::portsManualDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::portsManualDialog)
{
    ui->setupUi(this);
}

portsManualDialog::~portsManualDialog()
{
    delete ui;
}
