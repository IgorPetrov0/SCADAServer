#include "serialportsdialog.h"
#include "ui_serialportsdialog.h"

    serialPortsDialog::serialPortsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serialPortsDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("Настройка последовательных портов"));
    list=QSerialPortInfo::availablePorts();
    int size=list.size();
    ui->number_1->addItem(tr("Нет"),1000);
    ui->number_2->addItem(tr("Нет"),1000);
    for(int n=0;n!=size;n++){
        ui->number_1->addItem(list.at(n).portName(),n);
        ui->number_2->addItem(list.at(n).portName(),n);
    }
    ui->number_1->setCurrentIndex(0);
    ui->number_2->setCurrentIndex(0);
    port1=NULL;
    port2=NULL;

    ui->baudRate_1->setEnabled(false);
    ui->dataBits_1->setEnabled(false);
    ui->flowControl_1->setEnabled(false);
    ui->parity_1->setEnabled(false);
    ui->stopBits_1->setEnabled(false);

    ui->baudRate_2->setEnabled(false);
    ui->dataBits_2->setEnabled(false);
    ui->flowControl_2->setEnabled(false);
    ui->parity_2->setEnabled(false);
    ui->stopBits_2->setEnabled(false);


    fillComboBoxes();
    connect(ui->canselButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ui->okButton,SIGNAL(clicked(bool)),this,SLOT(saveSlot()));
    connect(ui->number_1,SIGNAL(currentIndexChanged(int)),this,SLOT(port1ChangeSlot(int)));
    connect(ui->number_2,SIGNAL(currentIndexChanged(int)),this,SLOT(port2ChangeSlot(int)));

}
///////////////////////////////////////////////////////////////////
serialPortsDialog::~serialPortsDialog()
{
    if(port1!=NULL){
        delete port1;
    }
    if(port2!=NULL){
        delete port2;
    }
    delete ui;
}
//////////////////////////////////////////////////////////////////////////////
QString serialPortsDialog::getPortName(int port){
    switch(port){
        case(1):{
            if(port1!=NULL){
                return port1->portName();
            }
            return "None";
        }
        case(2):{
            if(port2!=NULL){
                return port2->portName();
            }
            else{
                return "None";
            }
        }
        default:{
            qDebug("serialPortDialog::getPortName : Argument <number> may be 1 or 2");
            return "None";
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::setPort(QString name, int number){

    switch(number){
        case(1):{
            int size=ui->number_1->count();
            int n=0;
            for(n;n!=size;n++){
                if(ui->number_1->itemText(n)==name){
                    ui->number_1->setCurrentIndex(n);
                    break;
                }
            }
            if(n==size){
                QMessageBox box;
                box.setWindowTitle(tr("Ошибка"));
                box.setDefaultButton(QMessageBox::Ok);
                box.setText(tr("Порт ")+name+tr(" в системе не обнаружен. \n Подключите устройство или выберите другой порт."));
                box.exec();
                ui->number_1->setCurrentIndex(0);
            }
            break;
        }
        case(2):{
            int size=ui->number_2->count();
            int n=0;
            for(n;n!=size;n++){
                if(ui->number_2->itemText(n)==name){
                    ui->number_2->setCurrentIndex(n);
                    break;
                }
            }
            if(n==size){
                QMessageBox box;
                box.setWindowTitle(tr("Ошибка"));
                box.setDefaultButton(QMessageBox::Ok);
                box.setText(tr("Порт ")+name+tr(" в системе не обнаружен. \n Подключите устройство или выберите другой порт."));
                box.exec();
                ui->number_2->setCurrentIndex(0);
            }
            break;
        }
        default:{
            qDebug("serialPortDialog::setPort : Argument <number> may be 1 or 2");
        }
    }
}
////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::fillComboBoxes(){
    ui->baudRate_1->addItem("1200",QSerialPort::Baud1200);
    ui->baudRate_1->addItem("2400",QSerialPort::Baud2400);
    ui->baudRate_1->addItem("4800",QSerialPort::Baud4800);
    ui->baudRate_1->addItem("9600",QSerialPort::Baud9600);
    ui->baudRate_1->addItem("19200",QSerialPort::Baud19200);
    ui->baudRate_1->addItem("38400",QSerialPort::Baud38400);
    ui->baudRate_1->addItem("57600",QSerialPort::Baud57600);
    ui->baudRate_1->addItem("115200",QSerialPort::Baud115200);

    ui->baudRate_2->addItem("1200",QSerialPort::Baud1200);
    ui->baudRate_2->addItem("2400",QSerialPort::Baud2400);
    ui->baudRate_2->addItem("4800",QSerialPort::Baud4800);
    ui->baudRate_2->addItem("9600",QSerialPort::Baud9600);
    ui->baudRate_2->addItem("19200",QSerialPort::Baud19200);
    ui->baudRate_2->addItem("38400",QSerialPort::Baud38400);
    ui->baudRate_2->addItem("57600",QSerialPort::Baud57600);
    ui->baudRate_2->addItem("115200",QSerialPort::Baud115200);

    ui->dataBits_1->addItem("5",QSerialPort::Data5);
    ui->dataBits_1->addItem("6",QSerialPort::Data6);
    ui->dataBits_1->addItem("7",QSerialPort::Data7);
    ui->dataBits_1->addItem("8",QSerialPort::Data8);

    ui->dataBits_2->addItem("5",QSerialPort::Data5);
    ui->dataBits_2->addItem("6",QSerialPort::Data6);
    ui->dataBits_2->addItem("7",QSerialPort::Data7);
    ui->dataBits_2->addItem("8",QSerialPort::Data8);

    ui->parity_1->addItem("No parity",QSerialPort::NoParity);
    ui->parity_1->addItem("Even parity",QSerialPort::EvenParity);
    ui->parity_1->addItem("Odd parity",QSerialPort::OddParity);
    ui->parity_1->addItem("Space parity",QSerialPort::SpaceParity);
    ui->parity_1->addItem("Mark parity",QSerialPort::MarkParity);

    ui->parity_2->addItem("No parity",QSerialPort::NoParity);
    ui->parity_2->addItem("Even parity",QSerialPort::EvenParity);
    ui->parity_2->addItem("Odd parity",QSerialPort::OddParity);
    ui->parity_2->addItem("Space parity",QSerialPort::SpaceParity);
    ui->parity_2->addItem("Mark parity",QSerialPort::MarkParity);

    ui->stopBits_1->addItem("1",QSerialPort::OneStop);
    ui->stopBits_1->addItem("1.5",QSerialPort::OneAndHalfStop);
    ui->stopBits_1->addItem("2",QSerialPort::TwoStop);

    ui->stopBits_2->addItem("1",QSerialPort::OneStop);
    ui->stopBits_2->addItem("1.5",QSerialPort::OneAndHalfStop);
    ui->stopBits_2->addItem("2",QSerialPort::TwoStop);

    ui->flowControl_1->addItem(tr("Нет"),QSerialPort::NoFlowControl);
    ui->flowControl_1->addItem(tr("Аппаратный"),QSerialPort::HardwareControl);
    ui->flowControl_1->addItem(tr("Программный"),QSerialPort::SoftwareControl);

    ui->flowControl_2->addItem(tr("Нет"),QSerialPort::NoFlowControl);
    ui->flowControl_2->addItem(tr("Аппаратный"),QSerialPort::HardwareControl);
    ui->flowControl_2->addItem(tr("Программный"),QSerialPort::SoftwareControl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::port1ChangeSlot(int index){
    int data=ui->number_1->itemData(index).toInt();
    if(data!=1000){
        ui->baudRate_1->setEnabled(true);
        ui->dataBits_1->setEnabled(true);
        ui->flowControl_1->setEnabled(true);
        ui->parity_1->setEnabled(true);
        ui->stopBits_1->setEnabled(true);

        QSerialPortInfo info=list.at(data);
        port1 = new QSerialPort(info);
        int size=ui->baudRate_1->count();
        for(int n=0;n!=size;n++){
            if(ui->baudRate_1->itemData(n)==port1->baudRate()){
                ui->baudRate_1->setCurrentIndex(n);
                break;
            }
        }
        size=ui->dataBits_1->count();
        for(int n=0;n!=size;n++){
            if(ui->dataBits_1->itemData(n)==port1->dataBits()){
                ui->dataBits_1->setCurrentIndex(n);
                break;
            }
        }
        size=ui->flowControl_1->count();
        for(int n=0;n!=size;n++){
            if(ui->flowControl_1->itemData(n)==port1->flowControl()){
                ui->flowControl_1->setCurrentIndex(n);
                break;
            }
        }
        size=ui->parity_1->count();
        for(int n=0;n!=size;n++){
            if(ui->parity_1->itemData(n)==port1->parity()){
                ui->parity_1->setCurrentIndex(n);
                break;
            }
        }
        size=ui->stopBits_1->count();
        for(int n=0;n!=size;n++){
            if(ui->stopBits_1->itemData(n)==port1->stopBits()){
                ui->stopBits_1->setCurrentIndex(n);
                break;
            }
        }
        ui->nameLabel_1->setText(info.description());
    }
    else{
        if(port1!=NULL){
            delete port1;
            port1=NULL;
        }
        ui->baudRate_1->setEnabled(false);
        ui->dataBits_1->setEnabled(false);
        ui->flowControl_1->setEnabled(false);
        ui->parity_1->setEnabled(false);
        ui->stopBits_1->setEnabled(false);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::port2ChangeSlot(int index){
    int data=ui->number_2->itemData(index).toInt();
    if(data!=1000){
        ui->baudRate_2->setEnabled(true);
        ui->dataBits_2->setEnabled(true);
        ui->flowControl_2->setEnabled(true);
        ui->parity_2->setEnabled(true);
        ui->stopBits_2->setEnabled(true);

        QSerialPortInfo info=list.at(data);
        port2 = new QSerialPort(info);
        int size=ui->baudRate_2->count();
        for(int n=0;n!=size;n++){
            if(ui->baudRate_2->itemData(n)==port2->baudRate()){
                ui->baudRate_2->setCurrentIndex(n);
                break;
            }
        }
        size=ui->dataBits_2->count();
        for(int n=0;n!=size;n++){
            if(ui->dataBits_2->itemData(n)==port2->dataBits()){
                ui->dataBits_2->setCurrentIndex(n);
                break;
            }
        }
        size=ui->flowControl_2->count();
        for(int n=0;n!=size;n++){
            if(ui->flowControl_2->itemData(n)==port2->flowControl()){
                ui->flowControl_2->setCurrentIndex(n);
                break;
            }
        }
        size=ui->parity_2->count();
        for(int n=0;n!=size;n++){
            if(ui->parity_2->itemData(n)==port2->parity()){
                ui->parity_2->setCurrentIndex(n);
                break;
            }
        }
        size=ui->stopBits_2->count();
        for(int n=0;n!=size;n++){
            if(ui->stopBits_2->itemData(n)==port2->stopBits()){
                ui->stopBits_2->setCurrentIndex(n);
                break;
            }
        }
        ui->nameLabel_2->setText(info.description());
    }
    else{
        if(port2!=NULL){
            delete port2;
            port2=NULL;
        }
        ui->baudRate_2->setEnabled(false);
        ui->dataBits_2->setEnabled(false);
        ui->flowControl_2->setEnabled(false);
        ui->parity_2->setEnabled(false);
        ui->stopBits_2->setEnabled(false);
    }
}
////////////////////////////////////////////////////////////////////////////////
void serialPortsDialog::saveSlot(){
    if(port1!=NULL){
        port1->setBaudRate((QSerialPort::BaudRate)ui->baudRate_1->currentData().toInt());
        port1->setDataBits((QSerialPort::DataBits)ui->dataBits_1->currentData().toInt());
        port1->setFlowControl((QSerialPort::FlowControl)ui->flowControl_1->currentData().toInt());
        port1->setParity((QSerialPort::Parity)ui->parity_1->currentData().toInt());
        port1->setStopBits((QSerialPort::StopBits)ui->stopBits_1->currentData().toInt());
    }
    if(port2!=NULL){
        port2->setBaudRate((QSerialPort::BaudRate)ui->baudRate_2->currentData().toInt());
        port2->setDataBits((QSerialPort::DataBits)ui->dataBits_2->currentData().toInt());
        port2->setFlowControl((QSerialPort::FlowControl)ui->flowControl_2->currentData().toInt());
        port2->setParity((QSerialPort::Parity)ui->parity_2->currentData().toInt());
        port2->setStopBits((QSerialPort::StopBits)ui->stopBits_2->currentData().toInt());
    }
    accept();
}
