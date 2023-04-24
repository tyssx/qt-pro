#ifndef SETRCDIALOG_H
#define SETRCDIALOG_H

#include <QDialog>

class QAbstractButton;

namespace Ui
{
class SetRCDialog;
}

class SetRCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetRCDialog(QWidget *parent = nullptr);
    ~SetRCDialog();
    void getRowCol(int &row, int &col);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_toolButtonAdd_clicked();

    void on_toolButtonSub_clicked();

    void on_checkBoxIsSet_stateChanged(int arg1);

    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void OkSignal(int row, int col, QStringList strList);

private:
    Ui::SetRCDialog *ui;
    QStringList m_headList;
};

#endif // SETRCDIALOG_H
