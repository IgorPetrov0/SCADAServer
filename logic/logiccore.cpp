#include "logiccore.h"

logicCore::logicCore(statisticCore *pointer)
{
    statCorePointer=pointer;
}
/////////////////////////////////////////////////////////////////////
void logicCore::checkLogic(){
    int size=statCorePointer->getObjectsCount();
    for(int n=0;n!=size;n++){
        object *tmpObject=statCorePointer->getObjectForIndex(n);
        int portsCount=tmpObject->getPortsCount();
        for(int m=0;m!=portsCount;m++){
            objectPort *tmpPort=tmpObject->getPort(m);
            tmpPort->checkOnConditions();
            tmpPort->checkOffConditions();
        }
    }
}
