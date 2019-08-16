#ifndef DEFINES_H
#define DEFINES_H

#include <QTime>

#define STAT_PATH "/statistics"
#define CONFIG_FILE_NAME "config.cfg"
#define SP_CONFIG_FILE_NAME "spConfig.cfg"
#define NET_CONFIG_FILE_NAME "netConfig.cfg"
#define SIGNATURE "phConfig"
#define SPSIGNATURE "spConfig"
#define CUR_VERSION 0.0f
#define MAX_TCP_CONNECTIONS 3
#define DATA_STREAM_VERSION QDataStream::Qt_4_0
#define ON true
#define OFF false

//запросы
enum requestType{
    REQUEST_EMPTY=0,
    REQUEST_GET_DATA,
    REQUEST_CLEAR
};
//ответы
enum answerType{
    ANSWER_OK=0,
    ANSWER_ERROR,
    ANSWER_NO_DATA,
    ANSWER_CLEARED   
};

enum portTypes:int{
    PORT_INPUT,
    PORT_OUTPUT
};

//enum portState:int{
//    PORT_ON,
//    PORT_OFF
//};

enum objectType:int{
    objectMashine,
    objectUnknow
};

struct minutePoint{
    int value;
    int event;//событие
};

struct dayGraph{
    QString name;
    QDate date;
    minutePoint *minutesArray;
};

struct shift{
    QTime startTime;
    QTime stopTime;
};

enum serverCommand{
    SERVERCOMMAND_NO_COMMAND=0,
    SERVERCOMMAND_GET_STATISTIC,
    SERVERCOMMAND_EDIT_OBJECT,
    SERVERCOMMAND_CREATE_OBJECT,
    SERVERCOMMAND_DELETE_OBJECT
};

enum tcpPacketType{
    TCP_PACKET_COMMAND=0,
    TCP_PACKET_STATISTIC,
    TCP_PACKET_ANSWER,
    TCP_PACKET_ERROR
};

enum reportType{
    REPORT_DAY,
    REPORT_SHIFT,
    REPORT_ALL_MASHINES
};

enum eventType{
    EVENT_CONTROLLER_FAULT=0,
    EVENT_NO_TYPE,
    EVENT_NOT_READY,
    EVENT_NO_RESPONCE,
    EVENT_OK
};

enum logicType:int{
    LOGIC_OR,
    LOGIC_AND,
    LOGIC_NO
};

enum objectState:int{
    OBJECT_STATE_ON,    //включен - когда отвечает, но не производит продукцию
    OBJECT_STATE_OFF,   //выключен - когда не отвечает
    OBJECT_STATE_WORK,   //работает - когда производит продукцию
    OBJECT_STATE_ANY    //состояние не имеет значения
};

enum tableType:int{
    TABLE_PORTS=0,
    TABLE_ON_CONDITIONS,
    TABLE_OFF_CONDITIONS
};

#endif // DEFINES_H
