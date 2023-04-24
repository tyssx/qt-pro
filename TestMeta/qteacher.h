#ifndef QTEACHER_H
#define QTEACHER_H

#include "qperson.h"

class QTeacher : public QPerson
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit QTeacher(QString fName, QObject *parent = nullptr);

    enum
    {
        Type = TEA
    };
};

#endif // QTEACHER_H
