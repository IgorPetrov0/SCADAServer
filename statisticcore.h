#ifndef STATISTICCORE_H
#define STATISTICCORE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QApplication>
#include "errorprocessor.h"
#include "mashine.h"
#include "defines.h"
#include "reports/reportclass.h"
#include "reports/shiftreport.h"
#include "logic/objectport.h"



class statisticCore : public QObject, public errorProcessor
{
    Q_OBJECT
public:
    statisticCore(QObject *parent=nullptr);
    ~statisticCore();
    bool readConfiguration(QString workingDir);
    bool writeConfiguration(QString workingDir);
    int getObjectsCount();
    object *getObjectForIndex(int index);
    object *getObjectForName(QString name);
    int getObjectIndex(object *targetObject);
    bool createObject(QDataStream *str, bool remout=false);
    int getMashinsCount();
    mashine *getMashine(int index);
    bool isNameExist(QString name,object *ob=nullptr);//если ob задан, то проверяется все, кроме данного объекта
    object *getObjectForAddress(int address, object *ob=nullptr);
    //перемещение машины в списке
    bool mashineUp(int index);
    bool mashineDown(int index);
    //строит отчет по заданным датам
    reportClass *createReport(reportType type, QDate startDate, QDate stopDate, int mashineIndex);//за удаление отчета отвечает вызывающий
    void deleteObject(object *ob, bool objectOnly=false);
    dayGraph *readGraphFile(QString fullPath);//при использовании не забывать удалять полученные данные
    bool writeGraphsInFiles();//записывает графики всех активных машин в файлы
    QStringList getObjectsNamesList();
    void checkConditions();
    bool checkANDConditions(objectPort *port,bool on_off);
    bool checkORConditions(objectPort *port, bool on);
    bool checkNOTConditions(objectPort *port,bool on_off);
    void clearAll();

protected:
    QVector<mashine*> mashinesArray;
    void generateTestGraph(mashine *m);
    bool removeDirRecursively(QString dirPath);
    bool writeGraph(mashine *tmpMashine);
<<<<<<< HEAD
    bool generatePointersForConditions();
=======
    bool findObjectsForConditions();
>>>>>>> 9ee40cecae714103b6e03b277bd65aa2a1014df2

protected slots:
    void newDaySlot(mashine *tmpMashine);


signals:
    void consoleMessage(QString string);



};

#endif // STATISTICCORE_H
