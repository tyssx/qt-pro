#ifndef QSTUDENT_H
#define QSTUDENT_H

#include "qperson.h"

class QStudent : public QPerson
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit QStudent(QString fName, QObject *parent = nullptr);

    enum
    {
        Type = STU
    };
    virtual QString birth() override;
};

#endif // QSTUDENT_H
