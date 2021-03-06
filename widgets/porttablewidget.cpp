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
    QStringList headerList;
    headerList.append(tr("Номер"));
    headerList.append(tr("Описание"));
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
            item=new QTableWidgetItem(tmpPort->getDescription());
            item->setToolTip(tmpPort->getDescription());
            ui->tableWidget->setItem(n,1,item);
            item=new QTableWidgetItem(tmpPort->getTypeString());
            item->setToolTip(tmpPort->getDescription());
            ui->tableWidget->setItem(n,2,item);
        }
    }
    connect(ui->tableWidget,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(itemSelectSlot(int,int,int,int)));
    if((currentRow<ui->tableWidget->rowCount())&&(currentRow>=0)){//количество портов могло измениться и currentRow может не попасть в диаппазон
        ui->tableWidget->setCurrentCell(currentRow,0);
    }
    logicTableWidget::updateContent();
}
////////////////////////////////////////////////////////////////
void portTableWidget::setObject(object *newObject){
    currentObject=newObject;
}
/////////////////////////////////////////////////////////////////
void portTableWidget::itemSelectSlot(int row, int column, int prevRow, int prevCol){
    emit selectSignal(row);
}
