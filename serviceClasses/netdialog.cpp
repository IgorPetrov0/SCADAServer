#include "netdialog.h"
#include "ui_netdialog.h"

netDialog::netDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::netDialog)
{
    ui->setupUi(this);
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    setWindowTitle(tr("Настройка сети"));
}
/////////////////////////////////////////////////////////////////
netDialog::~netDialog()
{
    delete ui;
}
///////////////////////////////////////////////////////////////////////////////////////////
void netDialog::setPort(qint16 port){
    QString str=QString::number(port);
    if(!str.isEmpty()){
        ui->lineEdit->setText(str);
    }
}
//////////////////////////////////////////////////////////////////////////////
qint16 netDialog::getPort(){
    QString str=ui->lineEdit->text();
    if(!str.isEmpty()){
        return str.toInt();
    }
    return 0;
}
