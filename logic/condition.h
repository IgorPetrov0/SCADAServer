#ifndef CONDITION_H
#define CONDITION_H

#include <QTime>
#include <QString>
//#include "object.h"
//#include "logic/objectport.h"
//#include "defines.h"

class condition
{
public:
    condition();

protected:
    QString name;
    QString description;
    //object *targetObject;
    //objectPort *targetPort;
    bool portState;
    QTime time;
    logicType logic;

};

#endif // CONDITION_H
