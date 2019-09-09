#include "portsmanualdialog.h"
#include "ui_portsmanualdialog.h"

portsManualDialog::portsManualDialog(object *currentObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::portsManualDialog)
{
    this->currentObject=currentObject;
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    QStringList headerList;
    headerList.append(tr("Номер"));
    headerList.append(tr("Описание"));
    headerList.append(tr("Направление"));
    headerList.append(tr("Состояние"));
    ui->tableWidget->setHorizontalHeaderLabels(headerList);
    ui->objectNameLabel->setText(currentObject->getName());
    this->setWindowTitle(tr("Ручное управление портами"));
    connect(ui->exitButton,SIGNAL(clicked(bool)),this,SLOT(rejeced()));
    connect(ui->updateButton,SIGNAL(clicked(bool)),this,SLOT(updateSlot()));
    updateContent();
}
//////////////////////////////////////////////////////////////////////////////////
void portsManualDialog::updateContent(){
    if(currentObject!=nullptr){
        for(int n=0;n!=MAX_OBJECT_PORTS;n++){
            objectPort *tmpPort=currentObject->getPortByNumber(n);
            if(tmpPort!=nullptr){                                               //если порт определен
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(n));
                ui->tableWidget->setItem(n,0,item);
                item = new QTableWidgetItem(tmpPort->getDescription());
                ui->tableWidget->setItem(n,1,item);
                portTypes type=tmpPort->getType();
                QString tmpString;
                switch(type){
                    case(PORT_INPUT):{
                        tmpString=tr("Вход");
                        if(tmpPort->getState()){
                            item = new QTableWidgetItem(tr("Активен"));
                        }
                        else{
                            item = new QTableWidgetItem(tr("Не активен"));
                        }
                        ui->tableWidget->setItem(n,3,item);
                        break;
                    }
                    case(PORT_OUTPUT):{
                        tmpString=tr("Выход");
                        tablePushButton *button = new tablePushButton(n);
                        connect(button,SIGNAL(pushSignal(bool,int)),this,SLOT(changePortStateSlot(bool, int)));
                        button->setCheckable(true);
                        if(tmpPort->getState()){
                            button->setText(tr("Включен"));
                            button->setChecked(true);
                        }
                        else{
                            button->setText(tr("Выключен"));
                            button->setChecked(false);
                        }
                        ui->tableWidget->setCellWidget(n,3,button);
                        break;
                    }
                }
                item = new QTableWidgetItem(tmpString);
                ui->tableWidget->setItem(n,2,item);
            }
            else{//если не определен
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(n));
                ui->tableWidget->setItem(n,0,item);
                item = new QTableWidgetItem(tr("Не определен"));
                ui->tableWidget->setItem(n,1,item);
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////
portsManualDialog::~portsManualDialog(){
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////////////
void portsManualDialog::changePortStateSlot(bool state, int index){
    currentObject->getPort(index)->setState(state);
    updateContent();
}
/////////////////////////////////////////////////////////////////////////////////////
void portsManualDialog::updateSlot(){
    updateContent();
}
/////////////////////////////////////////////////////////////////////////////////
