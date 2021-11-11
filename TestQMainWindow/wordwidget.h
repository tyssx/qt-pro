#ifndef WORDWIDGET_H
#define WORDWIDGET_H

#include <QWidget>

namespace Ui
{
class WordWidget;
}

class WordWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WordWidget(QWidget *parent = nullptr);
    ~WordWidget();

private slots:
    void slot_changeForm(int status);
    void slot_changeColor();

private:
    Ui::WordWidget *ui;
};

#endif // WORDWIDGET_H
