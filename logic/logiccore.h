#ifndef LOGICCORE_H
#define LOGICCORE_H

#include "statisticcore.h"

class logicCore
{
public:
    logicCore(statisticCore *pointer);
    void checkLogic();



protected:
    statisticCore *statCorePointer;

};

#endif // LOGICCORE_H
