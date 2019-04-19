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

    QString getName() const;
    void setName(const QString &value);
    QString getDescription() const;
    void setDescription(const QString &value);
    object *getTargetObject() const;
    void setTargetObject(object *value);
    objectPort *getTargetPort() const;
    void setTargetPort(objectPort *value);
    bool getPortState() const;
    void setPortState(bool value);
    QTime getTime() const;
    void setTime(const QTime &value);
    logicType getLogic() const;
    QString getLogicString();
    void setLogic(const logicType &value);

protected:
    QString name;
    QString description;
    object *targetObject;
    objectPort *targetPort;
    bool portState;
    QTime time;
    logicType logic;

};

#endif // CONDITION_H
