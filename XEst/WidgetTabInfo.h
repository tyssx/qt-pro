#ifndef WIDGETTABINFO_H
#define WIDGETTABINFO_H

#include <QFrame>

namespace Ui {
class WidgetTabInfo;
}

class WidgetTabInfo : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetTabInfo(QWidget *parent = nullptr);
    ~WidgetTabInfo();

private:
    Ui::WidgetTabInfo *ui;
};

#endif // WIDGETTABINFO_H
