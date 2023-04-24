#include "qperson.h"

QPerson::QPerson(QString fName, QObject *parent) : QObject(parent)
{
    m_name = fName;
}

int QPerson::age()
{
    return m_age;
}

void QPerson::setAge(int value)
{
    m_age = value;
    emit ageChanged(value);
}

void QPerson::incAge()
{
    m_age++;
    emit ageChanged(m_age);
}

QString QPerson::birth()
{
    QString str = "20000101";
    return str;
}
