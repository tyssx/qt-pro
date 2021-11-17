#include "qstudent.h"

REGISTER_CLASS(QStudent);

QStudent::QStudent(QString fName, QObject *parent) : QPerson(fName, parent)
{
}

QString QStudent::birth()
{
    QString str = "20101001";
    return str;
}
