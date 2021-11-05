#ifndef WIDGETTFUNBTNS_H
#define WIDGETTFUNBTNS_H

#include <QFrame>
#include <QButtonGroup>

#include "XEstUiDef.h"

namespace Ui {
class WidgetTFunBtns;
}

class WidgetTFunBtns : public QFrame
{
    Q_OBJECT

public:
    explicit WidgetTFunBtns(QWidget *parent = nullptr);
    ~WidgetTFunBtns();

    void  hideButton(int Id); // 隐藏摸个不需要的按钮
public:
signals:
    void activeTargetPage(int nTargetPageId); //发出信号,将被激活页面的序号

public slots:
        void on_changeButtonGroup(int);

private:
    Ui::WidgetTFunBtns *ui;

    QButtonGroup *pBtnGroup;

};

#endif // WIDGETTFUNBTNS_H
