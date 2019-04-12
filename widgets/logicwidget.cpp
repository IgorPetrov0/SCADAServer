#include "logicwidget.h"
#include "ui_logicwidget.h"

logicWidget::logicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logicWidget)
{
    ui->setupUi(this);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(selectObjectSlot(int)));
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
            ui->comboBox->addItem(tmpObject->getName(),QVariant((uint)tmpObject));
        }
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
///////////////////////////////////////////////////////////////////////////
void logicWidget::selectObjectSlot(int index){
    if(index!=-1){
        object *tmpObject=(object*)ui->comboBox->itemData(index).toInt();
        if(tmpObject!=nullptr){
            ui->addressLabel->setText(tr("Адрес: ")+QString::number(tmpObject->getAddress()));
            ui->typeLabel->setText(tr("Тип: ")+tmpObject->getTypeString());
        }
    }
}
