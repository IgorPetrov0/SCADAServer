#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "statisticcore.h"
#include "basewidget.h"

namespace Ui {
class mainTabWidget;
}

class mainTabWidget : public QTabWidget, baseWidget
{
    Q_OBJECT

public:
    explicit mainTabWidget(QWidget *parent = 0);
    ~mainTabWidget();
    virtual void setStatCorePointer(statisticCore *pointer);
    virtual void updateContent();
    void showGraph(dayGraph *array);
    void writeToConsole(QString string);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void addObjectSignal();
    void editObjectSignal(objectType type, int index);
    void deleteObjectSignal(objectType type, int index);
    void createReportSygnal(reportType type, int index);
    void enotherDaySignal(int index);


private:
    Ui::mainTabWidget *ui;
};


#endif // MAINTABWIDGET_H
