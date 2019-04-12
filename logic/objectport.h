#ifndef OBJECTPORT_H
#define OBJECTPORT_H

#include <QString>
#include <QDataStream>

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

protected:
    int number;
    QString description;
    QString name;

};

#endif // OBJECTPORT_H
