#ifndef CHECKCOMBOBOX_H
#define CHECKCOMBOBOX_H

#include <QComboBox>
class QListWidget;

class CheckComboBox : public QComboBox
{
    Q_OBJECT
public:
    CheckComboBox(QWidget *parent = nullptr);
    ~CheckComboBox();

    void appendItem(const QString &text, bool bChecked); //添加下拉框内容
    void hidePopup(); // QComboBox的虚函数用来隐藏列表框,当单击是复选框时不让隐藏，用来改变状态

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    void updateIndexStatus(int index); //更新更改项的状态

protected slots:

signals:
    void checkedStateChange(int index, bool bChecked); //状态改变后发送一个信号，告诉外界。

private:
};

#endif // CHECKCOMBOBOX_H
