#ifndef WIDGETTDISPLAY_H
#define WIDGETTDISPLAY_H

#include <QFrame>

namespace Ui {
class WidgetTDisplay;
}

class WidgetTDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetTDisplay(QWidget *parent = nullptr);
    ~WidgetTDisplay();

private:
    Ui::WidgetTDisplay *ui;
};

#endif // WIDGETTDISPLAY_H
