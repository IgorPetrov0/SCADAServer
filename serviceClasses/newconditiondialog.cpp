#include "newconditiondialog.h"
#include "ui_newconditiondialog.h"

newConditionDialog::newConditionDialog(objectPort *port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newConditionDialog)
{
    ui->setupUi(this);
    currentPort=port;

    connect(ui->portCheckBox,SIGNAL(stateChanged(int)),this,SLOT(portCheckSlot(int)));
    connect(ui->stateCheckBox,SIGNAL(stateChanged(int)),this,SLOT(stateCheckSlot(int)));
    connect(ui->objectComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObject(int)));
    ui->portCheckBox->setCheckState(Qt::Checked);
    ui->stateCheckBox->setCheckState(Qt::Unchecked);
    ui->logicComboBox->addItem(tr("И"),LOGIC_AND);
    ui->logicComboBox->addItem(tr("ИЛИ"),LOGIC_OR);
    ui->logicComboBox->addItem(tr("НЕ"),LOGIC_NO);
    statCorePointer=nullptr;
}
////////////////////////////////////////////////////////////////////////////////
newConditionDialog::~newConditionDialog(){
    delete ui;
}
////////////////////////////////////////////////////////////////////////////////////
void newConditionDialog::setPortName(QString name){
    ui->portNameLabel->setText(tr("Порт: ")+name);
}
///////////////////////////////////////////////////////////////////////////////////
void newConditionDialog::setStatisticCorePointer(statisticCore *pointer){
    statCorePointer=pointer;
    if(pointer!=nullptr){
        int size=pointer->getObjectsCount();
        for(int n=0;n!=size;n++){
            ui->objectComboBox->addItem(pointer->getObjectForIndex(n)->getName());
        }
        ui->objectComboBox->setCurrentIndex(0);
    }

}
//////////////////////////////////////////////////////////////////////
void newConditionDialog::portCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portComboBox->setEnabled(true);
        ui->portStateComboBox->setEnabled(true);
        ui->stateComboBox->setEnabled(false);
        ui->stateCheckBox->setCheckState(Qt::Unchecked);
    }
}
////////////////////////////////////////////////////////////////////
void newConditionDialog::stateCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portComboBox->setEnabled(false);
        ui->portStateComboBox->setEnabled(false);
        ui->stateComboBox->setEnabled(true);
        ui->portCheckBox->setCheckState(Qt::Unchecked);
    }
}
/////////////////////////////////////////////////////////////////////////
void newConditionDialog::selectObject(int index){

}
