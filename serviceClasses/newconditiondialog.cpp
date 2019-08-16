#include "newconditiondialog.h"
#include "ui_newconditiondialog.h"

newConditionDialog::newConditionDialog(objectPort *port, QWidget *parent, int condIndex) :
    QDialog(parent),
    ui(new Ui::newConditionDialog)
{
    ui->setupUi(this);
    currentPort=port;

    connect(ui->portCheckBox,SIGNAL(stateChanged(int)),this,SLOT(portCheckSlot(int)));
    connect(ui->stateCheckBox,SIGNAL(stateChanged(int)),this,SLOT(stateCheckSlot(int)));
    connect(ui->objectComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObject(int)));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(okSlot()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    ui->portCheckBox->setCheckState(Qt::Checked);
    ui->stateCheckBox->setCheckState(Qt::Unchecked);
    ui->logicComboBox->addItem(tr("И"),LOGIC_AND);
    ui->logicComboBox->addItem(tr("ИЛИ"),LOGIC_OR);
    ui->logicComboBox->addItem(tr("НЕ"),LOGIC_NO);
    ui->stateComboBox->addItem(tr("Включен"),OBJECT_STATE_ON);
    ui->stateComboBox->addItem(tr("Отключен"),OBJECT_STATE_OFF);
    ui->stateComboBox->addItem(tr("Работает"),OBJECT_STATE_WORK);
    ui->portStateComboBox->addItem(tr("Включен"),true);
    ui->portStateComboBox->addItem(tr("Выключен"),false);
    statCorePointer=nullptr;
    currentCondition=nullptr;
}
////////////////////////////////////////////////////////////////////////////////
newConditionDialog::~newConditionDialog(){
    delete ui;
    if(currentCondition!=nullptr){
        delete currentCondition;
    }
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
//////////////////////////////////////////////////////////////////////////////////////
condition *newConditionDialog::getNewCondition(){
    return currentCondition;
}
//////////////////////////////////////////////////////////////////////
void newConditionDialog::portCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portComboBox->setEnabled(true);
        ui->portStateComboBox->setEnabled(true);
        ui->portCheckBox->setDisabled(true);
        ui->stateCheckBox->setEnabled(true);
        ui->stateComboBox->setDisabled(true);
        ui->stateCheckBox->setCheckState(Qt::Unchecked);
    }
}
////////////////////////////////////////////////////////////////////
void newConditionDialog::stateCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portComboBox->setDisabled(true);
        ui->portStateComboBox->setDisabled(true);
        ui->stateComboBox->setEnabled(true);
        ui->stateCheckBox->setDisabled(true);
        ui->portCheckBox->setEnabled(true);
        ui->portCheckBox->setCheckState(Qt::Unchecked);
    }
}
/////////////////////////////////////////////////////////////////////////
void newConditionDialog::selectObject(int index){
    ui->portComboBox->clear();
    object *tmpObject=statCorePointer->getObjectForIndex(index);
    int size=tmpObject->getPortsCount();
    for(int n=0;n!=size;n++){
        ui->portComboBox->addItem(tmpObject->getPort(n)->getName());
    }
}
//////////////////////////////////////////////////////////////////////////
void newConditionDialog::okSlot(){
    currentCondition = new condition;
    currentCondition->setDescription(ui->descriptionTextEdit->document()->toPlainText());
    currentCondition->setLogic((logicType)ui->logicComboBox->currentData().toInt());
    object *tmpObject=statCorePointer->getObjectForIndex(ui->objectComboBox->currentIndex());
    currentCondition->setTargetObjectName(tmpObject->getName());
    if(ui->portCheckBox->isChecked()){
        currentCondition->setTargetPortName(tmpObject->getPort(ui->portComboBox->currentIndex())->getName());
        currentCondition->setPortState(ui->portStateComboBox->currentData().toBool());
        currentCondition->setTargetObjectState(OBJECT_STATE_ANY);
    }
    else if(ui->stateCheckBox->isChecked()){
        currentCondition->setTargetPortName("");
        currentCondition->setPortState(false);
        currentCondition->setTargetObjectState((objectState)ui->stateComboBox->currentData().toInt());
    }
    currentCondition->setTime(ui->timeSpinBox->value());
    accept();
}
