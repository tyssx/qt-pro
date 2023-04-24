#ifndef SPORTCONTROL_H
#define SPORTCONTROL_H

#include <QObject>
#include "zauxdll2.h"

class SportControl : public QObject
{
    Q_OBJECT
public:
    explicit SportControl(QObject *parent = nullptr);

signals:

public slots:
    void singleAxisSportSlot(void *g_handle,int iaxis, int distance);
    void leftRightOriginSlot(void *g_handle,int imode);
    void autoSportSlot(void *g_handle);
//    void stopAutoSportSlot(void *g_handle);
};

#endif // SPORTCONTROL_H
