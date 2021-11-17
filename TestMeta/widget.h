#ifndef WIDGET_H
#define WIDGET_H

#include "qperson.h"

#include <QWidget>

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonBoyAdd_clicked();

    void on_pushButtonGirlAdd_clicked();

    void on_pushButtonMetaInfo_clicked();

    void on_pushButtonClearText_clicked();

    void slot_ageChanged(int value);
    void slot_valueChanged(int value);

    void on_pushButtonResStu_clicked();

    void on_pushButtonRegTea_clicked();

private:
    Ui::Widget *ui;
    QPerson *boy;
    QPerson *girl;
};

#endif // WIDGET_H
