#ifndef OBJECTPORT_H
#define OBJECTPORT_H

#include <QString>
#include <QDataStream>
#include <QVector>
#include <QObject>
#include "logic/condition.h"
#include "defines.h"


class objectPort
{
public:
    objectPort();
    int getNumber() const;
    void setNumber(int value);
    QString getDescription() const;
    void setDescription(const QString &value);
    QString getName() const;
    void setName(const QString &value);
    void serialisation(QDataStream *str);
    void deserialisation(QDataStream *str);
    int getOnConditionsCount();
    int getOffConditionsCount();
    condition *getOnCondition(int index);
    condition *getOffCondition(int index);
    portTypes getType() const;
    QString getTypeString();
    void setType(const portTypes &value);

protected:
    int number;
    QString description;
    QString name;
    portTypes type;
    QVector<condition*>onConditions;
    QVector<condition*>offConditions;



};

#endif // OBJECTPORT_H
