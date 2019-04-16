#include "ontablewidget.h"
#include "ui_logictablewidget.h"

onTableWidget::onTableWidget(QWidget *parent):
    logicTableWidget(parent)
{
    ui->groupBox->setTitle(tr("Включить, если:"));
    type=TABLE_ON_CONDITIONS;
}
