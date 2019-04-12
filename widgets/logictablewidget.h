#ifndef LOGICTABLEWIDGET_H
#define LOGICTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class logicTableWidget;
}

class logicTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit logicTableWidget(QWidget *parent = 0);
    ~logicTableWidget();

protected slots:
    void addSlot();
    void editSlot();
    void deleteSlot();

protected:
    void resizeEvent(QResizeEvent *event);
    Ui::logicTableWidget *ui;
};

#endif // LOGICTABLEWIDGET_H
