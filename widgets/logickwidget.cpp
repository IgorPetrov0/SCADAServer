#include "logickwidget.h"
#include "ui_logickwidget.h"

logickWidget::logickWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logickWidget)
{
    ui->setupUi(this);
}

logickWidget::~logickWidget()
{
    delete ui;
}
