#include "setrcdialog.h"

#include "ui_setrcdialog.h"

#include <QAbstractButton>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

SetRCDialog::SetRCDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SetRCDialog)
{
    ui->setupUi(this);
    ui->checkBoxIsSet->setChecked(true);
}

SetRCDialog::~SetRCDialog()
{
    delete ui;
}

void SetRCDialog::getRowCol(int &row, int &col)
{
    row = ui->spinBoxRow->value();
    col = ui->spinBoxColumn->value();
}

void SetRCDialog::closeEvent(QCloseEvent *event)
{
    if (event->isAccepted()) { qDebug() << "判断接收"; }
}

void SetRCDialog::on_toolButtonAdd_clicked()
{
    if (ui->lineEditHead->text().isEmpty()) return;
    ui->comboBoxHeadList->addItem(ui->lineEditHead->text());
    m_headList << ui->lineEditHead->text();
}

void SetRCDialog::on_toolButtonSub_clicked()
{
    int index = ui->comboBoxHeadList->currentIndex();
    ui->comboBoxHeadList->removeItem(index);
    m_headList.removeAt(index);
}

void SetRCDialog::on_checkBoxIsSet_stateChanged(int arg1)
{
    if (arg1)
    {
        ui->comboBoxHeadList->setEnabled(true);
        ui->lineEditHead->setEnabled(true);
        ui->toolButtonAdd->setEnabled(true);
        ui->toolButtonSub->setEnabled(true);
    }
    else
    {
        ui->comboBoxHeadList->setEnabled(false);
        ui->lineEditHead->setEnabled(false);
        ui->toolButtonAdd->setEnabled(false);
        ui->toolButtonSub->setEnabled(false);
    }
}

void SetRCDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QPushButton *pushBtn = qobject_cast<QPushButton *>(button);
    if (pushBtn == ui->buttonBox->button(QDialogButtonBox::Ok))
    {
        if (ui->checkBoxIsSet->checkState())
        {
            int count = ui->comboBoxHeadList->count();
            if (count != ui->spinBoxColumn->value())
            {
                QMessageBox::critical(this, "提醒", "输入的表头数量不匹配");
                return;
            }
        }
        else
        {
            m_headList.clear();
        }
        emit OkSignal(ui->spinBoxRow->value(), ui->spinBoxColumn->value(), m_headList);
    }
}
