#include "offtablewidget.h"
#include "ui_logictablewidget.h"

offTableWidget::offTableWidget(QWidget *parent):
    logicTableWidget(parent)
{
    ui->groupBox->setTitle(tr("Отключить, если:"));
    type=TABLE_OFF_CONDITIONS;
}
