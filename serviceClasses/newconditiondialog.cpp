#include "newconditiondialog.h"
#include "ui_newconditiondialog.h"

newConditionDialog::newConditionDialog(statisticCore *pointer, objectPort *port, QWidget *parent, bool on, int condIndex) :
    QDialog(parent),
    ui(new Ui::newConditionDialog)
{
    ui->setupUi(this);
    currentPort=port;
    ui->portNameLabel->setText(QString::number(port->getNumber())+"  "+port->getDescription());

    if(on){
        setWindowTitle(tr("Условие включения"));
    }
    else{
        setWindowTitle(tr("Условие выключения"));
    }

    statCorePointer=pointer;
    if(pointer==nullptr){
        qDebug("newConditionDialog::newConditionDialog statisticCore pointer in nullptr");
        return;
    }
    int size=pointer->getObjectsCount();
    for(int n=0;n!=size;n++){
        ui->objectComboBox->addItem(pointer->getObjectForIndex(n)->getName());
    }
    ui->objectComboBox->setCurrentIndex(-1);


    connect(ui->portCheckBox,SIGNAL(stateChanged(int)),this,SLOT(portCheckSlot(int)));
    connect(ui->stateCheckBox,SIGNAL(stateChanged(int)),this,SLOT(stateCheckSlot(int)));
    connect(ui->objectComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObject(int)));
    connect(ui->portNumberComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectPortSlot(int)));
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

    ui->objectComboBox->setCurrentIndex(0);


    if(condIndex>=0){
        if(on){
            currentCondition=currentPort->getOnCondition(condIndex);
        }
        else{
            currentCondition=currentPort->getOffCondition(condIndex);
        }
        viewCurrentCondition();
        editMode=true;
    }
    else{
        currentCondition = new condition;
        editMode=false;
    }
}
////////////////////////////////////////////////////////////////////////////////
newConditionDialog::~newConditionDialog(){
    delete ui;
    if(!editMode){
        if(currentCondition!=nullptr){
            delete currentCondition;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////
void newConditionDialog::setPortNumber(int number){
    ui->portNameLabel->setText(tr("Порт: ")+QString::number(number));
}
//////////////////////////////////////////////////////////////////////////////////////
condition *newConditionDialog::getNewCondition(){
    condition *tmpCond=currentCondition;
    currentCondition=nullptr;
    return tmpCond;
}
///////////////////////////////////////////////////////////////////////////////////
void newConditionDialog::viewCurrentCondition(){
    ui->descriptionTextEdit->setText(currentCondition->getDescription());

    //логика
    switch(currentCondition->getLogic()){
        case(LOGIC_AND):{
            ui->logicComboBox->setCurrentIndex(0);
            break;
        }
        case(LOGIC_OR):{
            ui->logicComboBox->setCurrentIndex(1);
            break;
        }
        case(LOGIC_NO):{
            ui->logicComboBox->setCurrentIndex(2);
            break;
        }
    }

    //объект
    int index=statCorePointer->getObjectIndex(currentCondition->getTargetObject());
    if(index!=-1){
        ui->objectComboBox->setCurrentIndex(index);
    }
    else{
        ui->objectComboBox->setCurrentIndex(0);
    }

    //порт или состояние
    if(currentCondition->getTargetObjectState()==OBJECT_STATE_ANY){//если порт
        ui->portCheckBox->setCheckState(Qt::Checked);
        ui->stateCheckBox->setCheckState(Qt::Unchecked);
        ui->stateComboBox->setDisabled(true);
        ui->portNumberComboBox->setEnabled(true);
        int n=currentCondition->getTargetPortNumber();
        int portIndex=currentCondition->getTargetObject()->getPortIndex(n);
        ui->portNumberComboBox->setCurrentIndex(portIndex);
        ui->portStateComboBox->setEnabled(true);
        if(currentCondition->getPortState()){
            ui->portStateComboBox->setCurrentIndex(0);
        }
        else{
            ui->portStateComboBox->setCurrentIndex(1);
        }
    }
    else{
        ui->portCheckBox->setCheckState(Qt::Unchecked);
        ui->stateCheckBox->setCheckState(Qt::Checked);
        ui->stateComboBox->setEnabled(true);
        ui->portNumberComboBox->setDisabled(true);
        ui->portStateComboBox->setDisabled(true);
        switch(currentCondition->getTargetObjectState()){
            case(OBJECT_STATE_ON):{
                ui->stateComboBox->setCurrentIndex(0);
                break;
            }
            case(OBJECT_STATE_OFF):{
                ui->stateComboBox->setCurrentIndex(1);
                break;
            }
            case(OBJECT_STATE_WORK):{
                ui->stateComboBox->setCurrentIndex(2);
                break;
            }
            default:{
                ui->stateComboBox->setCurrentIndex(0);
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
void newConditionDialog::portCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portNumberComboBox->setEnabled(true);
        ui->portStateComboBox->setEnabled(true);
        ui->stateComboBox->setDisabled(true);
        ui->stateCheckBox->setCheckState(Qt::Unchecked);
    }
}
////////////////////////////////////////////////////////////////////
void newConditionDialog::stateCheckSlot(int state){
    if(state==Qt::Checked){
        ui->portNumberComboBox->setDisabled(true);
        ui->portStateComboBox->setDisabled(true);
        ui->stateComboBox->setEnabled(true);
        ui->portCheckBox->setCheckState(Qt::Unchecked);
    }
}
/////////////////////////////////////////////////////////////////////////
void newConditionDialog::selectObject(int index){
    disconnect(ui->portNumberComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectPortSlot(int)));
    ui->portNumberComboBox->clear();
    ui->portDecrLine->clear();
    object *tmpObject=statCorePointer->getObjectForIndex(index);
    int size=tmpObject->getPortsCount();
    for(int n=0;n!=size;n++){
        int number=tmpObject->getPort(n)->getNumber();
        ui->portNumberComboBox->addItem(QString::number(number));
    }
    ui->portNumberComboBox->setCurrentIndex(-1);
    connect(ui->portNumberComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectPortSlot(int)));
    ui->portNumberComboBox->setCurrentIndex(0);
}
///////////////////////////////////////////////////////////////////////////
void newConditionDialog::selectPortSlot(int index){
    object *tmpObject=statCorePointer->getObjectForIndex(ui->objectComboBox->currentIndex());
    ui->portDecrLine->setText(tmpObject->getPort(index)->getDescription());
}
//////////////////////////////////////////////////////////////////////////
void newConditionDialog::okSlot(){
    currentCondition->setDescription(ui->descriptionTextEdit->document()->toPlainText());
    currentCondition->setLogic((logicType)ui->logicComboBox->currentData().toInt());
    object *tmpObject=statCorePointer->getObjectForIndex(ui->objectComboBox->currentIndex());
    currentCondition->setTargetObject(tmpObject);
    if(ui->portCheckBox->isChecked()){
        currentCondition->setPortState(ui->portStateComboBox->currentData().toBool());
        currentCondition->setTargetObjectState(OBJECT_STATE_ANY);
        currentCondition->setTargetPort(tmpObject->getPort(ui->portNumberComboBox->currentIndex()));
    }
    else if(ui->stateCheckBox->isChecked()){
        currentCondition->setTargetPortNumber(-1);
        currentCondition->setPortState(false);
        currentCondition->setTargetObjectState((objectState)ui->stateComboBox->currentData().toInt());
    }
    currentCondition->setTime(ui->timeSpinBox->value());

    accept();
}
//////////////////////////////////////////////////////////////////////////////////

