#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateTimer = new QTimer(this);
    saveTimer = new QTimer(this);
    connect(ui->actionNewObject,SIGNAL(triggered(bool)),this,SLOT(createNewObjectSlot()));
    connect(ui->mainTab,SIGNAL(addObjectSignal()),this,SLOT(createNewObjectSlot()));
    connect(ui->mainTab,SIGNAL(editObjectSignal(objectType,int)),this,SLOT(editObjectSlot(objectType,int)));
    connect(ui->mainTab,SIGNAL(deleteObjectSignal(objectType,int)),this,SLOT(deleteObjectSlot(objectType,int)));
    connect(ui->openGraphAction,SIGNAL(triggered(bool)),this,SLOT(openGraphSlot(int)));
    connect(ui->actionSerialPorts,SIGNAL(triggered(bool)),this,SLOT(serialPortsSlot()));
    connect(ui->mainTab,SIGNAL(createReportSygnal(reportType,int)),this,SLOT(createReport(reportType,int)));
    connect(ui->mainTab,SIGNAL(enotherDaySignal(int)),this,SLOT(openGraphSlot(int)));
    connect(ui->actionNetSetting,SIGNAL(triggered(bool)),this,SLOT(netSettingSlot()));
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(updateTimeSlot()));
    connect(this->saveTimer,SIGNAL(timeout()),this,SLOT(saveTimeSlot()));
    connect(&statCore,SIGNAL(consoleMessage(QString)),this,SLOT(writeConsoleSlot(QString)));
    connect(&rCore,SIGNAL(consoleMessage(QString)),this,SLOT(writeConsoleSlot(QString)));
    connect(&netCore,SIGNAL(consoleMessage(QString)),this,SLOT(writeConsoleSlot(QString)));
    appPath=QApplication::applicationDirPath();
    rCore.setStatisticCorePointer(&statCore);
    saveTime=180000;
    saveTimer->setSingleShot(true);
    saveTimer->start(saveTime);
    updateTimer->start(WINDOW_UPDATE_TIME);
}
////////////////////////////////////////////////////////////////
MainWindow::~MainWindow()
{ 
    updateTimer->stop();
    delete updateTimer;
    if(!statCore.writeConfiguration(appPath)){
        errorMessage(statCore.getLastError());
    }
    if(!rCore.writeConfiguration(appPath)){
        errorMessage(statCore.getLastError());
    }
    delete ui;
}
/////////////////////////////////////////////////////////////////
void MainWindow::resizeEvent(QResizeEvent *event){
    ui->mainTab->setGeometry(this->centralWidget()->geometry());
}
////////////////////////////////////////////////////////////////////////
void MainWindow::createNewObjectSlot(){
    newObjectMasterBase master;
    master.setStatCorePointer(&statCore);
    if(master.exec()==QDialog::Accepted){
        QByteArray arr=master.getObjectData();
        QDataStream str(&arr,QIODevice::ReadOnly);
        if(!statCore.createObject(&str)){
            errorMessage(statCore.getLastError());
        }
        ui->mainTab->updateContent();
        statCore.writeConfiguration(appPath);
    }
}
///////////////////////////////////////////////////////////////////////////////////
void MainWindow::editObjectSlot(objectType type, int index){
    newObjectMasterBase master;
    object *tmp=NULL;
    switch(type){
        case(objectMashine):{
            tmp=statCore.getMashine(index);
            break;
        }
        default:{
            qDebug("MainWindow::editObjectSlot : Unknow object type.");
            return;
        }
    }
    if(tmp==NULL){
        errorMessage(tr("Внутренняя ошибка. Указанного объекта не существует."));
        return;
    }
    master.loadObject(tmp);
    master.setStatCorePointer(&statCore);
    if(master.exec()==QDialog::Accepted){
        ui->mainTab->updateContent();
        statCore.writeConfiguration(appPath);
    }
}
////////////////////////////////////////////////////////////////////////////////
void MainWindow::deleteObjectSlot(objectType type, int index){
    updateTimer->stop();
    object *tmp=NULL;
    switch(type){
        case(objectMashine):{
            tmp=statCore.getMashine(index);
            break;
        }
        default:{
            qDebug("MainWindow::editObjectSlot : Unknow object type.");
            return;
        }
    }
    if(tmp!=NULL){
        deleteObjectDialog dialog(this,tmp);
        switch(dialog.exec()){
            case(DELETE_OBJECT_ONLY):{
                statCore.deleteObject(tmp,true);
                break;
            }
            case(DELETE_ALL):{
                statCore.deleteObject(tmp,false);
                break;
            }
            case(QDialog::Rejected):{
                return;
                break;
            }
        }
        ui->mainTab->updateContent();
    }
    else{
        qDebug("MainWindow::deleteObjectSlot : Object is NULL");
    }
    updateTimer->start(WINDOW_UPDATE_TIME);

}
///////////////////////////////////////////////////////////////////////////////
void MainWindow::openGraphSlot(int index){
    QFileDialog dialog(this);
    if(index==-1){
        dialog.setDirectory(QApplication::applicationDirPath());
    }
    else{
        mashine *tmpMashine=statCore.getMashine(index);
        if(tmpMashine!=NULL){
            QString str=tmpMashine->getPathForStatistics();
            dialog.setDirectory(tmpMashine->getPathForStatistics());
        }
        else{
            dialog.setDirectory(QApplication::applicationDirPath());
        }
    }
    if(dialog.exec()==QDialog::Accepted){
        QStringList list=dialog.selectedFiles();
        dayGraph *dayStruct = statCore.readGraphFile(list.at(0));
        if(dayStruct==NULL){
            errorMessage(statCore.getLastError());
            return;
        }
        ui->mainTab->showGraph(dayStruct);
    }
}
///////////////////////////////////////////////////////////////////////////////////
void MainWindow::serialPortsSlot(){
    serialPortsDialog dialog(this);
    QSerialPort *port=rCore.getPort(1);
    if(port!=NULL){
        dialog.setPort(port->portName(),1);
    }
    port=rCore.getPort(2);
    if(port!=NULL){
        dialog.setPort(port->portName(),2);
    }
    if(dialog.exec()==QDialog::Accepted){
        if(!rCore.setPort(dialog.getPortName(1),1)){
            errorMessage(rCore.getLastError());
        }
        if(!rCore.setPort(dialog.getPortName(2),2)){
            errorMessage(rCore.getLastError());
        }
        if(!rCore.writeConfiguration(appPath)){
            errorMessage(rCore.getLastError());
        }
    }
}
////////////////////////////////////////////////////////////////////////////
void MainWindow::createReport(reportType type, int index){
    kalendarDialog dialog(this);
    dialog.setWindowTitle(tr("Выберите диапазон дат"));
    if(dialog.exec()==QDialog::Accepted){
        reportClass *report=statCore.createReport(type,dialog.getStartDate(),dialog.getStopDate(),index);
        if(report==NULL){
            errorMessage(statCore.getLastError());
            return;
        }
        reportDialog rDialog(this);
        rDialog.showReport(report);
        rDialog.exec();
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::updateTimeSlot(){
    ui->mainTab->updateContent();
}
////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::saveTimeSlot(){
    statCore.writeGraphsInFiles();
    saveTimer->start(saveTime);
}
///////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::netSettingSlot(){
    netDialog dialog;
    dialog.setPort(netCore.serverPort());
    if(dialog.exec()==QDialog::Accepted){
        QMessageBox box;
        box.addButton(QMessageBox::Yes);
        box.addButton(QMessageBox::No);
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("Внимание!\n При смене порта все соединения будут отключены \n"
                       "и потребуется перенастройка номера порта на клиентских терминалах.\n"
                       "Вы готовы продолжить?"));
        if(box.exec()==QMessageBox::Yes){
            netCore.setNetPort(dialog.getPort());
            if(!netCore.writeConfiguration(appPath)){
                errorMessage(netCore.getLastError());
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindow::writeConsoleSlot(QString string){
    QString dateTime=QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")+"  ";
    ui->mainTab->writeToConsole(dateTime+string);
}
//////////////////////////////////////////////////////////////////////////////////
void MainWindow::initialise(){
    ui->mainTab->setStatCorePointer(&statCore);
    QString appPath=QApplication::applicationDirPath();
    QDir statDir=appPath+STAT_PATH;//проверяем наличие папки со статистикой
    if(!statDir.exists()){
        if(!statDir.mkdir(appPath+STAT_PATH)){//если не нашли, то создаем
            errorMessage(tr("Невозможно создать новую директорию в рабочей директории программы."));
        }
    }
    QFile file(appPath+"/"+CONFIG_FILE_NAME);//ищем файл конфигурации
    if(!file.exists()){
        if(!file.open(QIODevice::WriteOnly)){//если не нашли, то создаем
            errorMessage(tr("Невозможно создать файл конфигурации в рабочей директории программы."));
        }
        file.close();
    }
    if(!statCore.readConfiguration(appPath)){
        errorMessage(statCore.getLastError());
    }
    if(!rCore.readConfiguration(appPath)){
        errorMessage(rCore.getLastError());
    }
    if(!netCore.readConfiguration(appPath)){
        errorMessage(netCore.getLastError());
    }
    netCore.setStatisticCorePointer(&statCore);
    ui->mainTab->updateContent();
}
///////////////////////////////////////////////////////////////////////////////////
void MainWindow::errorMessage(QString error){
    writeConsoleSlot(error);
    QMessageBox box(this);
    box.setDefaultButton(QMessageBox::Ok);
    box.setWindowTitle(tr("Ошибка"));
    box.setText(error);
    box.exec();
}
///////////////////////////////////////////////////////////////////////////////////
