#include "checkcombobox.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>

CheckComboBox::CheckComboBox(QWidget *parent) : QComboBox(parent)
{
}
CheckComboBox::~CheckComboBox()
{
}

void CheckComboBox::appendItem(const QString &text, bool bChecked)
{
    QIcon icon;
    if (bChecked) { icon.addFile(":/Image/check.png"); }
    else
    {
        icon.addFile(":/Image/uncheck.png");
    }
    addItem(icon, text, bChecked);
}

void CheckComboBox::hidePopup()
{
    int iconWidth = this->iconSize().width();

    int x = QCursor::pos().x() - mapToGlobal(geometry().topLeft()).x() + geometry().x();

    int index = view()->selectionModel()->currentIndex().row();

    if (x >= 0 && x <= iconWidth) { updateIndexStatus(index); }
    else
    {
        QComboBox::hidePopup();
    }
}

void CheckComboBox::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x();

    int iconWidth = this->iconSize().width();

    if (x <= iconWidth)
    {
        int index = this->currentIndex();

        updateIndexStatus(index);
    }
    else
    {
        QComboBox::mousePressEvent(event);
    }
}

void CheckComboBox::updateIndexStatus(int index)
{
    bool bChecked = this->itemData(index).toBool();
    if (bChecked) { this->setItemIcon(index, QIcon(":/Image/uncheck.png")); }
    else
    {
        this->setItemIcon(index, QIcon(":/Image/check.png"));
    }
    setItemData(index, !bChecked);
    emit checkedStateChange(index, !bChecked);
}
