#ifndef LOGICKWIDGET_H
#define LOGICKWIDGET_H

#include <QWidget>

namespace Ui {
class logickWidget;
}

class logickWidget : public QWidget
{
    Q_OBJECT

public:
    explicit logickWidget(QWidget *parent = 0);
    ~logickWidget();

private:
    Ui::logickWidget *ui;
};

#endif // LOGICKWIDGET_H
