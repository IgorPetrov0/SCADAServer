#ifndef CONDITION_H
#define CONDITION_H

#include <QTime>
#include <QString>
#include <QObject>
#include "defines.h"


class object;
class objectPort;
class statisticCore;

class condition
{

public:
    condition();

    QString getDescription() const;
    void setDescription(const QString &value);
    QString getTargetObjectName() const;
    void setTargetObjectName(QString name);
    int getTargetPortNumber() const;
    void setTargetPortNumber(int number);
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
    bool deserialisation(QDataStream *str);
    condition &operator=(const condition *right);
    object *getTargetObject() const;
<<<<<<< HEAD
    objectPort *getTargetPort() const;
    bool generateTargetPointers(statisticCore *statCorePointer);//использовать после полной загрузки файла CONFIG_FILE_NAME

=======
    bool findObjectPort(statisticCore *statCorePointer);
>>>>>>> 9ee40cecae714103b6e03b277bd65aa2a1014df2
protected:
    QString description;
    QString targetObjectName;
    object *targetObject;
    objectPort *targetPort;
    int targetPortNumber;
    bool portState;
    objectState targetObjectState;
    int time;
    logicType logic;




};

#endif // CONDITION_H
