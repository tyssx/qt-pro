#ifndef TRANSFER_H
#define TRANSFER_H

#include <QObject>

class Transfer : public QObject
{
    Q_OBJECT
public:
    explicit Transfer(QObject *parent = nullptr);
    bool Grab();
    bool Freeze();

signals:

public slots:

};

#endif // TRANSFER_H
