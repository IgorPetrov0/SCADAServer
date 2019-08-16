#include "logicwidget.h"
#include "ui_logicwidget.h"

logicWidget::logicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logicWidget)
{
    ui->setupUi(this);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObjectSlot(int)));
    connect(ui->portWidget,SIGNAL(addSignal(tableType)),this,SLOT(addSlot(tableType)));
    connect(ui->portWidget,SIGNAL(editSignal(tableType,int)),this,SLOT(editSlot(tableType,int)));
    connect(ui->portWidget,SIGNAL(deleteSignal(tableType,int)),this,SLOT(deleteSlot(tableType,int)));
    connect(ui->offWidget,SIGNAL(addSignal(tableType)),this,SLOT(addSlot(tableType)));
    connect(ui->offWidget,SIGNAL(editSignal(tableType,int)),this,SLOT(editSlot(tableType,int)));
    connect(ui->offWidget,SIGNAL(deleteSignal(tableType,int)),this,SLOT(deleteSlot(tableType,int)));
    connect(ui->onWidget,SIGNAL(addSignal(tableType)),this,SLOT(addSlot(tableType)));
    connect(ui->onWidget,SIGNAL(editSignal(tableType,int)),this,SLOT(editSlot(tableType,int)));
    connect(ui->onWidget,SIGNAL(deleteSignal(tableType,int)),this,SLOT(deleteSlot(tableType,int)));
    connect(ui->portWidget,SIGNAL(selectSignal(int)),this,SLOT(selectPortSlot(int)));
    currentObjectPointer=nullptr;
    currentPortPointer=nullptr;
    ui->onWidget->setTableType(TABLE_ON_CONDITIONS);
    ui->offWidget->setTableType(TABLE_OFF_CONDITIONS);
}
////////////////////////////////////////////////////
logicWidget::~logicWidget()
{
    delete ui;
}
/////////////////////////////////////////////////////////////////
void logicWidget::resizeEvent(QResizeEvent *event){
    QRect rect;
    int width=(this->width()-20)/3;
    rect.setX(5);
    rect.setY(75);
    rect.setWidth(width);
    rect.setHeight(this->height()-rect.y());
    ui->portWidget->setGeometry(rect);

    rect.setX(rect.x()+rect.width()+5);
    rect.setY(75);
    rect.setWidth(width);
    rect.setHeight(this->height()-rect.y());
    ui->onWidget->setGeometry(rect);

    rect.setX(rect.x()+rect.width()+5);
    rect.setY(75);
    rect.setWidth(width);
    rect.setHeight(this->height()-rect.y());
    ui->offWidget->setGeometry(rect);
}
//////////////////////////////////////////////////////////////////////
void logicWidget::updateContent(){
    if(statCorePointer!=nullptr){
        disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObjectSlot(int)));
        int size=statCorePointer->getObjectsCount();
        int currentIndex=ui->comboBox->currentIndex();
        ui->comboBox->clear();
        for(int n=0;n!=size;n++){
            object *tmpObject=statCorePointer->getObjectForIndex(n);
            ui->comboBox->addItem(tmpObject->getName());
        }
        connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObjectSlot(int)));
        if((currentIndex>=0)&&(currentIndex<ui->comboBox->count())){
            ui->comboBox->setCurrentIndex(currentIndex);
        }
        else{
            ui->comboBox->setCurrentIndex(0);
        }
    }
    else{
        qDebug("logicWidget::updateContent  statCorePointer is null");
    }
}
////////////////////////////////////////////////////////////////////////////
void logicWidget::clear(){
    currentObjectPointer=nullptr;
    ui->offWidget->setCurrentPort(nullptr);
    ui->onWidget->setCurrentPort(nullptr);
    ui->portWidget->setObject(nullptr);
}
///////////////////////////////////////////////////////////////////////////
void logicWidget::selectObjectSlot(int index){
    if(index!=-1){
        currentObjectPointer=statCorePointer->getObjectForIndex(index);
        if(currentObjectPointer!=nullptr){
            ui->addressLabel->setText(tr("Адрес: ")+QString::number(currentObjectPointer->getAddress()));
            ui->typeLabel->setText(tr("Тип: ")+currentObjectPointer->getTypeString());
            ui->portWidget->setObject(currentObjectPointer);
            ui->portWidget->updateContent();
            if(currentPortPointer==nullptr){
                selectPortSlot(0);
            }
        }  
    } 
}
////////////////////////////////////////////////////////////////////////////////////////
void logicWidget::selectPortSlot(int index){
    currentPortPointer=currentObjectPointer->getPort(index);
    if(currentPortPointer!=nullptr){
        ui->onWidget->setCurrentPort(currentPortPointer);
        ui->onWidget->updateContent();
        ui->offWidget->setCurrentPort(currentPortPointer);
        ui->offWidget->updateContent();
    }
    else{
        ui->onWidget->clear();
        ui->offWidget->clear();
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////
void logicWidget::addSlot(tableType type){
    switch(type){
        case(TABLE_PORTS):{
            newPortDialog dialog(currentObjectPointer,this);
            if(dialog.exec()==QDialog::Accepted){
                currentObjectPointer->addPort(dialog.getPort());
            }
            break;
        }
        case(TABLE_ON_CONDITIONS):{
            if(currentPortPointer!=nullptr){
                if(currentPortPointer->getType()==PORT_OUTPUT){//условия только для выходов
                    newConditionDialog dialog(currentPortPointer,this);
                    dialog.setWindowTitle(tr("Условие включения"));
                    dialog.setPortName(currentPortPointer->getName());
                    dialog.setStatisticCorePointer(statCorePointer);
                    if(dialog.exec()==QDialog::Accepted){
                        currentPortPointer->addOnCondition(dialog.getNewCondition());
                        ui->onWidget->updateContent();
                    }
                }
                else{//если текущий вход
                    QMessageBox box(this);
                    box.setText(tr("Выбранный порт является входом.\n"
                                   "Нет смысла задавать условие для входа.\n"
                                   "Выберите выходной порт для задания условий включения/выключения."));
                    box.setWindowTitle("Ошибка");
                    box.setIcon(QMessageBox::Critical);
                    box.setStandardButtons(QMessageBox::Ok);
                    box.exec();
                }
            }
            break;
        }
        case(TABLE_OFF_CONDITIONS):{
            if(currentPortPointer!=nullptr){
                if(currentPortPointer->getType()==PORT_OUTPUT){//условия только для выходов
                    newConditionDialog dialog(currentPortPointer,this);
                    dialog.setWindowTitle(tr("Условие отключения"));
                    dialog.setPortName(currentPortPointer->getName());
                    dialog.setStatisticCorePointer(statCorePointer);
                    if(dialog.exec()==QDialog::Accepted){
                        currentPortPointer->addOffCondition(dialog.getNewCondition());
                        ui->offWidget->updateContent();
                    }
                }
                QMessageBox box(this);
                box.setText(tr("Выбранный порт является входом.\n"
                               "Нет смысла задавать условие для входа.\n"
                               "Выберите выходной порт для задания условий включения/выключения."));
                box.setWindowTitle("Ошибка");
                box.setIcon(QMessageBox::Critical);
                box.setStandardButtons(QMessageBox::Ok);
                box.exec();
            }
            break;
        }
    }
    updateContent();
}
/////////////////////////////////////////////////////////////////////////
void logicWidget::editSlot(tableType type, int index){
    if(index>=0){
        switch(type){
            case(TABLE_PORTS):{
                newPortDialog dialog(currentObjectPointer,this);
                dialog.setPort(currentObjectPointer->getPort(index));
                dialog.exec();
                break;
            }
            case(TABLE_ON_CONDITIONS):{
                newConditionDialog dialog(currentPortPointer,this);
                break;
            }
            case(TABLE_OFF_CONDITIONS):{

                break;
            }
        }
    }
    updateContent();
}
////////////////////////////////////////////////////////////////////////
void logicWidget::deleteSlot(tableType type, int index){
    if(index>=0){
        QMessageBox box;
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.setIcon(QMessageBox::Question);
        switch(type){
            case(TABLE_PORTS):{
                box.setWindowTitle(tr("Удаление порта."));
                box.setText(tr("Порт ")+QString::number(currentObjectPointer->getPort(index)->getNumber())+
                            tr(" объекта ")+currentObjectPointer->getName()+tr(" будет удален. Вы уверены?"));
                if(box.exec()==QMessageBox::Yes){
                    currentObjectPointer->removePort(index);
                }
                break;
            }
            case(TABLE_ON_CONDITIONS):{

                break;
            }
            case(TABLE_OFF_CONDITIONS):{

                break;
            }
        }
        updateContent();
    }
}
