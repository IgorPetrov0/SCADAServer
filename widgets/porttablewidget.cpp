#include "porttablewidget.h"
#include "ui_logictablewidget.h"

portTableWidget::portTableWidget(QWidget *parent):
    logicTableWidget(parent)
{
    ui->groupBox->setTitle(tr("Порт:"));
    type=TABLE_PORTS;
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(itemSelectSlot(int,int,int,int)));
    currentObject=nullptr;
}
////////////////////////////////////////////////////////////
void portTableWidget::updateContent(){
    int currentRow=ui->tableWidget->currentRow();
    disconnect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(itemSelectSlot(int,int,int,int)));
    ui->tableWidget->clear();
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(itemSelectSlot(int,int,int,int)));
    QStringList headerList;
    headerList.append(tr("Номер"));
    headerList.append(tr("Название"));
    headerList.append(tr("Тип"));
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(headerList);

    if(currentObject!=nullptr){
        int size=currentObject->getPortsCount();
        ui->tableWidget->setRowCount(size);
        for(int n=0;n!=size;n++){
            objectPort *tmpPort=currentObject->getPort(n);
            QTableWidgetItem *item=new QTableWidgetItem(QString::number(tmpPort->getNumber()));
            item->setToolTip(tmpPort->getDescription());
            ui->tableWidget->setItem(n,0,item);
            item=new QTableWidgetItem(tmpPort->getName());
            item->setToolTip(tmpPort->getDescription());
            ui->tableWidget->setItem(n,1,item);
            item=new QTableWidgetItem(tmpPort->getTypeString());
            item->setToolTip(tmpPort->getDescription());
            ui->tableWidget->setItem(n,2,item);
        }
    }
    ui->tableWidget->setCurrentCell(currentRow,0);
}
////////////////////////////////////////////////////////////////
void portTableWidget::setObject(object *newObject){
    currentObject=newObject;
}
/////////////////////////////////////////////////////////////////
void portTableWidget::itemSelectSlot(int row, int column, int prevRow, int prevCol){
    emit selectSignal(row);
}
