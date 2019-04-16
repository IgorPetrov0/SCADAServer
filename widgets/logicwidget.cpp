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
    currentObjectPointer=nullptr;
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
        int size=statCorePointer->getObjectsCount();
        int currentIndex=ui->comboBox->currentIndex();
        ui->comboBox->clear();
        for(int n=0;n!=size;n++){
            object *tmpObject=statCorePointer->getObjectForIndex(n);
            ui->comboBox->addItem(tmpObject->getName());
        }
        if((currentIndex>=0)&&(currentIndex<ui->comboBox->count())){
            ui->comboBox->setCurrentIndex(currentIndex);
        }
        else{
            ui->comboBox->setCurrentIndex(0);
        }
        ui->portWidget->updateContent();
    }
    else{
        qDebug("logicWidget::updateContent  statCorePointer is null");
    }
}
///////////////////////////////////////////////////////////////////////////
void logicWidget::selectObjectSlot(int index){
    if(index!=-1){
        currentObjectPointer=statCorePointer->getObjectForIndex(index);
        if(currentObjectPointer!=nullptr){
            ui->addressLabel->setText(tr("Адрес: ")+QString::number(currentObjectPointer->getAddress()));
            ui->typeLabel->setText(tr("Тип: ")+currentObjectPointer->getTypeString());
            ui->portWidget->setObject(currentObjectPointer);
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////
void logicWidget::selectPortSlot(int index){

}
///////////////////////////////////////////////////////////////////////////////////////////////
void logicWidget::addSlot(tableType type){
    switch(type){
        case(TABLE_PORTS):{
            newPortDialog dialog(currentObjectPointer);
            if(dialog.exec()==QDialog::Accepted){
                updateContent();
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
}
/////////////////////////////////////////////////////////////////////////
void logicWidget::editSlot(tableType type, int index){
    switch(type){
        case(TABLE_PORTS):{

            break;
        }
        case(TABLE_ON_CONDITIONS):{

            break;
        }
        case(TABLE_OFF_CONDITIONS):{

            break;
        }
    }
}
////////////////////////////////////////////////////////////////////////
void logicWidget::deleteSlot(tableType type, int index){
    switch(type){
        case(TABLE_PORTS):{

            break;
        }
        case(TABLE_ON_CONDITIONS):{

            break;
        }
        case(TABLE_OFF_CONDITIONS):{

            break;
        }
    }
}
