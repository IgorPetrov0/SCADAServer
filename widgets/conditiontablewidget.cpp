#include "conditiontablewidget.h"
#include "ui_logictablewidget.h"

conditionTableWidget::conditionTableWidget(QWidget *parent):
    logicTableWidget(parent)
{

}
///////////////////////////////////////////////////////////////////////
void conditionTableWidget::clear(){
    logicTableWidget::clear();
    ui->tableWidget->setColumnCount(6);
    QStringList headerList;
    headerList.append(tr("Логика"));
    headerList.append(tr("Объект"));
    headerList.append(tr("Состояние"));
    headerList.append(tr("Порт"));
    headerList.append(tr("Состояние"));
    headerList.append(tr("Время"));
    ui->tableWidget->setHorizontalHeaderLabels(headerList);
}
/////////////////////////////////////////////////////////////////////
void conditionTableWidget::updateContent(){
    int currentRow=ui->tableWidget->currentRow();
    clear();
    switch(type){//если условия включения
        case(TABLE_ON_CONDITIONS):{
            ui->groupBox->setTitle(tr("Включить, если: "));
            int size=currentPort->getOnConditionsCount();
            ui->tableWidget->setRowCount(size);
            for(int n=0;n!=size;n++){
                condition *tmpCond=currentPort->getOnCondition(n);
                fillTableString(tmpCond,n);
            }
            break;
        }
        case(TABLE_OFF_CONDITIONS):{
            ui->groupBox->setTitle(tr("Выключить, если: "));
            int size=currentPort->getOffConditionsCount();
            ui->tableWidget->setRowCount(size);
            for(int n=0;n!=size;n++){
                condition *tmpCond=currentPort->getOffCondition(n);
                fillTableString(tmpCond,n);
            }
            break;
        }
        default:{
            qDebug("conditionTableWidget::updateContent()  wrong table type");
        }
    }
    if((currentRow<ui->tableWidget->rowCount())&&(currentRow>=0)){//количество портов могло измениться и currentRow может не попасть в диаппазон
        ui->tableWidget->setCurrentCell(currentRow,0);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
void conditionTableWidget::fillTableString(condition *tmpCond, int row){
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(tmpCond->getLogicString());
    ui->tableWidget->setItem(row,0,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getTargetObjectName());
    ui->tableWidget->setItem(row,1,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getTargetObjectStateString());
    ui->tableWidget->setItem(row,2,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getTargetPortName());
    ui->tableWidget->setItem(row,3,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getPortStateString());
    ui->tableWidget->setItem(row,4,item);

    item = new QTableWidgetItem;
    item->setText(QString::number(tmpCond->getTime()));
    ui->tableWidget->setItem(row,5,item);
}