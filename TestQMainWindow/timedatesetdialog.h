#ifndef TIMEDATESETDIALOG_H
#define TIMEDATESETDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui
{
class TimeDateSetDialog;
}

class TimeDateSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeDateSetDialog(QWidget *parent = nullptr);
    ~TimeDateSetDialog();

private slots:
    void on_pushButtonGetCurTime_clicked();

    void on_pushButtonSetDate_clicked();

    void on_pushButtonSetTime_clicked();

    void on_pushButtonSetDateTime_clicked();

    void on_calendarWidget_selectionChanged();

    void slot_timer_timeout();

    void on_pushButtonTimeBegin_clicked();

    void on_pushButtonTimeStop_clicked();

    void on_pushButtonSetPeriod_clicked();

private:
    Ui::TimeDateSetDialog *ui;
    QTimer *m_timer;
    QTime *m_time;
};

#endif // TIMEDATESETDIALOG_H
