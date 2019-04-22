#ifndef CONDITION_H
#define CONDITION_H

#include <QTime>
#include <QString>
#include <QObject>
#include "defines.h"


class object;
class objectPort;

class condition
{
public:
    condition();

    QString getDescription() const;
    void setDescription(const QString &value);
    QString getTargetObjectName() const;
    void setTargetObjectName(QString name);
    QString getTargetPortName() const;
    void setTargetPortName(QString name);
    bool getPortState() const;
    QString getPortStateString();
    void setPortState(bool value);
    int getTime() const;
    void setTime(const int value);
    logicType getLogic() const;
    QString getLogicString();
    void setLogic(const logicType &value);
    objectState getTargetObjectState() const;
    QString getTargetObjectStateString();
    void setTargetObjectState(const objectState &value);
    void serialisation(QDataStream *str);
    void deserialisation(QDataStream *str);

protected:
    QString description;
    QString targetObjectName;
    QString targetPortName;
    bool portState;
    objectState targetObjectState;
    int time;
    logicType logic;

};

#endif // CONDITION_H
