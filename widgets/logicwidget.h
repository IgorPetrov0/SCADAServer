#ifndef LOGICKWIDGET_H
#define LOGICKWIDGET_H

#include <QWidget>
#include "basewidget.h"

namespace Ui {
class logicWidget;
}

class logicWidget : public QWidget, public baseWidget
{
    Q_OBJECT

public:
    explicit logicWidget(QWidget *parent = 0);
    ~logicWidget();
    virtual void updateContent();

protected:
    void resizeEvent(QResizeEvent *event);


protected slots:
    void selectObjectSlot(int index);

private:
    Ui::logicWidget *ui;
};

#endif // LOGICKWIDGET_H
