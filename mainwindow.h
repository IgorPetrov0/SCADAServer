#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "statisticcore.h"
#include "defines.h"
#include "newObjectMaster/newobjectmasterbase.h"
#include "serviceClasses/deleteobjectdialog.h"
#include "serviceClasses/serialportsdialog.h"
#include "serviceClasses/kalendardialog.h"
#include "serviceClasses/reportdialog.h"
#include "serviceClasses/netdialog.h"
#include "requestcore.h"
#include "netserver.h"


#define WINDOW_UPDATE_TIME 10000


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initialise();
    ~MainWindow();

protected:
    statisticCore statCore;
    requestCore rCore;
    netServer netCore;
    int saveTime;

    QString appPath;
    QTimer *updateTimer;
    QTimer *saveTimer;
    void resizeEvent(QResizeEvent *event);
    void errorMessage(QString error);

protected slots:
    void createNewObjectSlot();
    void editObjectSlot(objectType type, int index);
    void deleteObjectSlot(objectType type, int index);
    void openGraphSlot(int index=-1);
    void serialPortsSlot();
    void createReport(reportType type,int index);
    void updateTimeSlot();
    void saveTimeSlot();
    void netSettingSlot();
    void writeConsoleSlot(QString string);

private:
    Ui::MainWindow *ui;




};

#endif // MAINWINDOW_H
