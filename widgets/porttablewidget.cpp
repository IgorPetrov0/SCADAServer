#include "porttablewidget.h"
#include "ui_logictablewidget.h"

portTableWidget::portTableWidget(QWidget *parent):
    logicTableWidget(parent)
{
    ui->groupBox->setTitle(tr("Порт:"));
}
