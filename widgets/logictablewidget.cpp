#include "logictablewidget.h"
#include "ui_logictablewidget.h"

logicTableWidget::logicTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logicTableWidget)
{
    ui->setupUi(this);
    connect(ui->addButton,SIGNAL(clicked(bool)),this,SLOT(addSlot()));
    connect(ui->editButton,SIGNAL(clicked(bool)),this,SLOT(editSlot()));
    connect(ui->deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteSlot()));
}
////////////////////////////////////////////////////////////////////////////////
void logicTableWidget::setCurrentPort(objectPort *port){
    currentPort=port;
}
///////////////////////////////////////////////////////////////////////////////
void logicTableWidget::updateContent(){
    int t=0;
}
//////////////////////////////////////////////////////////////////////////
void logicTableWidget::setTableType(tableType type){
    this->type=type;
}
/////////////////////////////////////////////////////////////////////////////
void logicTableWidget::clear(){
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}
////////////////////////////////////////////////////////////////////////////
logicTableWidget::~logicTableWidget()
{
    delete ui;
}
/////////////////////////////////////////////////////////////////////////
void logicTableWidget::addSlot(){
    emit addSignal(type);
}
//////////////////////////////////////////////////////////////////////
void logicTableWidget::editSlot(){
    emit editSignal(type,ui->tableWidget->currentRow());
}
///////////////////////////////////////////////////////////////////////
void logicTableWidget::deleteSlot(){
    emit deleteSignal(type,ui->tableWidget->currentRow());
}
//////////////////////////////////////////////////////////////////////////////
void logicTableWidget::resizeEvent(QResizeEvent *event){
    QRect rect;
    rect.setX(1);
    rect.setY(1);
    rect.setWidth(this->width()-1);
    rect.setHeight(this->height()-3);
    ui->groupBox->setGeometry(rect);

    rect.setX(20);
    rect.setY(20);
    rect.setWidth(this->width()-40);
    rect.setHeight(this->height()-70);
    ui->tableWidget->setGeometry(rect);

    rect.setX(20);
    rect.setY(rect.y()+rect.height()+5);
    rect.setWidth(70);
    rect.setHeight(23);
    ui->addButton->setGeometry(rect);

    rect.setX(this->width()/2-45);
    rect.setWidth(90);
    ui->editButton->setGeometry(rect);

    rect.setX(this->width()-70-20);
    rect.setWidth(70);
    ui->deleteButton->setGeometry(rect);

}
