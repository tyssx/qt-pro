#include "qteacher.h"

REGISTER_CLASS(QTeacher)

QTeacher::QTeacher(QString fName, QObject *parent) : QPerson(fName, parent)
{
}
