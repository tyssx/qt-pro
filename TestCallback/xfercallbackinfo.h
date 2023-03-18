#ifndef XFERCALLBACKINFO_H
#define XFERCALLBACKINFO_H

#include <QObject>
#include "transfer.h"

class XferCallbackInfo : public QObject
{
    Q_OBJECT
public:
    explicit XferCallbackInfo(QObject *parent = nullptr);
    XferCallbackInfo(Transfer *pXfer, void *context, bool isTrash) { m_pXfer = pXfer; m_Context = context; m_bTrash = isTrash; }

    void *GetContext() const { return m_Context; }
signals:

public slots:


protected:
    Transfer *m_pXfer;
    void *m_Context;
    bool m_bTrash;
};

#endif // XFERCALLBACKINFO_H
