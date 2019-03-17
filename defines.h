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
    SERVERCOMMAND_GET_STATISTIC
};

enum tcpPacketType{
    TCP_PACKET_COMMAND=0,
    TCP_PACKET_STATISTIC
};

enum reportType{
    REPORT_DAY,
    REPORT_SHIFT,
    REPORT_ALL_MASHINES
};

enum eventType{
    EVENT_CONTROLLER_FAULT,
    EVENT_NO_TYPE,
    EVENT_NOT_READY,
    EVENT_NO_RESPONCE,
    EVENT_OK
};


#endif // DEFINES_H
