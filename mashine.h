#ifndef MASHINE_H
#define MASHINE_H

#include <QDataStream>
#include <QColor>
#include <QDateTime>
#include <QVector>
#include "object.h"


class mashine : public object
{
    Q_OBJECT

public:
    mashine();
    ~mashine();
    virtual void serialisation(QDataStream *str);
    virtual void deserialisation(QDataStream *str);
    virtual void deserialisationContinue(QDataStream *str);
    void serialiseDayGraph(QDataStream *str);
    mashine& operator=(const object& right);
    QColor getLineColor() const;
    int getShiftsCount();
    shift getShift(int number);
    void setLineColor(const QColor &value);
    QString getPathForStatistics() const;
    void setPathForStatistics(const QString &value);
    void addShift(shift newShift);
    void addTimePoint(minutePoint point, QTime time);
    dayGraph *getCurrentGraph();
    void readPacket(unsigned char *array,QTime time);
    void memoryNotCleared();//вызывается, если небыло ответа на запрос стирания памяти

protected:
    QColor lineColor;
    QVector<shift> shiftsArray;
    QString pathForStatistics;
    dayGraph *currentDayGraph;
    QTime lastRequestTime;
    bool notCleared;

};

#endif // MASHINE_H
