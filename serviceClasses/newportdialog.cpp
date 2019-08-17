#include "newportdialog.h"
#include "ui_newportdialog.h"

newPortDialog::newPortDialog(object *currentObject, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newPortDialog)
{
    ui->setupUi(this);
    this->currentObject=currentObject;
    currentPort=nullptr;
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(okSlot()));
    connect(ui->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}
//////////////////////////////////////////////////////////////
newPortDialog::~newPortDialog()
{
    delete ui;
}
///////////////////////////////////////////////////////////////////
void newPortDialog::setPort(objectPort *port){
    currentPort=port;
    ui->descriptionLineEdit->setText(port->getDescription());
    ui->numberSpinBox->setValue(port->getNumber());
}
//////////////////////////////////////////////////////////////////////
objectPort *newPortDialog::getPort(){
    return currentPort;
}
/////////////////////////////////////////////////////////////////////
void newPortDialog::errorMessage(QString message){
    QMessageBox box(this);
    box.setWindowTitle(tr("Ошибка"));
    box.setStandardButtons(QMessageBox::Ok);
    box.setText(message);
    box.setIcon(QMessageBox::Warning);
    box.exec();
}
////////////////////////////////////////////////////////////////
void newPortDialog::okSlot(){

    int value=ui->numberSpinBox->value();
    if(currentObject->isPortExist(value,currentPort)){
        errorMessage(tr("Объект ")+currentObject->getName()+tr(" уже использут порт с номером ")+
                     QString::number(currentObject->getPort(value)->getNumber()));
        return;
    }

    if(currentPort==nullptr){
        currentPort = new objectPort;
    }
    currentPort->setNumber(value);
    currentPort->setDescription(ui->descriptionLineEdit->text());
    switch(ui->typeComboBox->currentIndex()){
        case(0):{
            currentPort->setType(PORT_INPUT);
            break;
        }
        case(1):{
            currentPort->setType(PORT_OUTPUT);
            break;
        }
    }
    accept();
}
//////////////////////////////////////////////////////////////
