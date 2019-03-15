#ifndef STATISTICCORE_H
#define STATISTICCORE_H

#include <QVector>
#include <QFile>
#include <QDir>
#include <QObject>
#include "errorprocessor.h"
#include "mashine.h"
#include "defines.h"
#include "reports/reportclass.h"
#include "reports/shiftreport.h"



class statisticCore : public QObject, public errorProcessor
{
    Q_OBJECT
public:
    statisticCore(QObject *parent=NULL);
    ~statisticCore();
    bool readConfiguration(QString workingDir);
    bool writeConfiguration(QString workingDir);
    int getObjectsCount();
    object *getObjectForIndex(int index);
    bool createObject(QDataStream *str);
    int getMashinsCount();
    mashine *getMashine(int index);
    bool isNameExist(QString name,object *ob=NULL);//если ob задан, то проверяется все, кроме данного объекта
    object *getObjectForAddress(int address, object *ob=NULL);
    //перемещение машины в списке
    bool mashineUp(int index);
    bool mashineDown(int index);
    //строит отчет по заданным датам
    reportClass *createReport(reportType type, QDate startDate, QDate stopDate, int mashineIndex);//за удаление отчета отвечает вызывающий
    void deleteObject(object *ob, bool objectOnly=false);
    dayGraph *readGraphFile(QString fullPath);//при использовании не забывать удалять полученные данные
    bool writeGraphsInFiles();//записывает графики всех активных машин в файлы

protected:
    QVector<mashine*> mashinesArray;
    void generateTestGraph(mashine *m);


signals:
    void consoleMessage(QString string);



};

#endif // STATISTICCORE_H
