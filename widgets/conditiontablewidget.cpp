#include "conditiontablewidget.h"
#include "ui_logictablewidget.h"

conditionTableWidget::conditionTableWidget(QWidget *parent):
    logicTableWidget(parent)
{

}
/////////////////////////////////////////////////////////////////////
void conditionTableWidget::updateContent(){
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(7);
    QStringList headerList;
    headerList.append(tr("Логика"));
    headerList.append(tr("Состояние"));
    headerList.append(tr("Порт"));
    headerList.append(tr("Объект"));
    headerList.append(tr("Время"));
    ui->tableWidget->setHorizontalHeaderLabels(headerList);

    switch(type){//если условия включения
        case(TABLE_ON_CONDITIONS):{
            int size=currentPort->getOnConditionsCount();
            ui->tableWidget->setRowCount(size);
            for(int n=0;n!=size;n++){
                condition *tmpCond=currentPort->getOnCondition(n);
                fillTableString(tmpCond,n);
            }
            break;
        }
        case(TABLE_OFF_CONDITIONS):{
            int size=currentPort->getOnConditionsCount();
            ui->tableWidget->setRowCount(size);
            for(int n=0;n!=size;n++){
                condition *tmpCond=currentPort->getOnCondition(n);
                fillTableString(tmpCond,n);
            }
            break;
        }
        default:{
            qDebug("conditionTableWidget::updateContent()  wrong table type");
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////
void conditionTableWidget::fillTableString(condition *tmpCond, int row){
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(tmpCond->getLogicString());
    ui->tableWidget->setItem(row,1,item);

    item = new QTableWidgetItem;
    if(tmpCond->getTargetPort()->getState()==true){
        item->setText(tr("Включен"));
    }
    else{
        item->setText(tr("Выключен"));
    }
    ui->tableWidget->setItem(row,4,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getTargetObject()->getName());
    ui->tableWidget->setItem(row,2,item);

    item = new QTableWidgetItem;
    item->setText(QString::number(tmpCond->getTargetPort()->getNumber()));
    ui->tableWidget->setItem(row,3,item);

    item = new QTableWidgetItem;
    item->setText(QString::number(tmpCond->getTargetPort()->getNumber()));
    ui->tableWidget->setItem(row,3,item);

    item = new QTableWidgetItem;
    item->setText(tmpCond->getTime().toString("mm:hh"));
    ui->tableWidget->setItem(row,3,item);

}
