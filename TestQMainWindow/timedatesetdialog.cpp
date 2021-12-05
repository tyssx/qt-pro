#include "timedatesetdialog.h"

#include "ui_timedatesetdialog.h"

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

TimeDateSetDialog::TimeDateSetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::TimeDateSetDialog)
{
    ui->setupUi(this);
    m_time = new QTime;
    ui->progressBar->setValue(0);
    ui->pushButtonSetPeriod->setEnabled(false);
    ui->pushButtonTimeStop->setEnabled(false);

    QDate date = QDate::currentDate();
    ui->labelCurDateTime->setText(QString::asprintf("今天是%04d年%02d月%02d日", date.year(), date.month(), date.day()));
    QTime time = QTime::currentTime();                                  //获取当前时间
    ui->lcdNumberHour->display(QString::asprintf("%02d", time.hour())); // LCD显示,不足两位数补0
    ui->lcdNumberMinute->display(QString::asprintf("%02d", time.minute()));
    ui->lcdNumberSecond->display(QString::asprintf("%02d", time.second()));

    m_timer = new QTimer(this);
    m_timer->setInterval(1000); //设置超时间隙
    connect(m_timer, &QTimer::timeout, this, &TimeDateSetDialog::slot_timer_timeout);
    m_timer->start(); //定时器开始
}

TimeDateSetDialog::~TimeDateSetDialog()
{
    delete ui;
}

void TimeDateSetDialog::on_pushButtonGetCurTime_clicked()
{
    QDateTime curDataTime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(curDataTime);
    ui->lineEditDateTime->setText(curDataTime.toString("yyyy-MM-dd hh:mm:ss"));
    ui->dateEdit->setDate(curDataTime.date());
    ui->lineEditDate->setText(curDataTime.date().toString("yyyy年MM月dd日"));
    ui->timeEdit->setTime(curDataTime.time());
    ui->lineEditTime->setText(curDataTime.time().toString("hh时mm分ss秒"));
}

void TimeDateSetDialog::on_pushButtonSetDate_clicked()
{
    QString str = ui->lineEditDate->text();
    str         = str.trimmed(); //去掉首尾空格
    if (!str.isEmpty())
    {
        QDate date = QDate::fromString(str, "yyyy年MM月dd日");
        ui->dateEdit->setDate(date);
    }
}

void TimeDateSetDialog::on_pushButtonSetTime_clicked()
{
    QString str = ui->lineEditTime->text();
    str         = str.trimmed(); //去掉首尾空格
    if (!str.isEmpty())
    {
        QTime time = QTime::fromString(str, "hh时mm分ss秒");
        ui->timeEdit->setTime(time);
    }
}

void TimeDateSetDialog::on_pushButtonSetDateTime_clicked()
{
    QString str = ui->lineEditDateTime->text();
    str         = str.trimmed(); //去掉首尾空格
    if (!str.isEmpty())
    {
        QDateTime dateTime = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss");
        ui->dateTimeEdit->setDateTime(dateTime);
    }
}

void TimeDateSetDialog::on_calendarWidget_selectionChanged()
{
    QDate date = ui->calendarWidget->selectedDate();
    ui->lineEditChoiceCalendar->setText(date.toString("yyyy年MM月dd日"));
}

void TimeDateSetDialog::slot_timer_timeout()
{
    QTime time = QTime::currentTime();                                  //获取当前时间
    ui->lcdNumberHour->display(QString::asprintf("%02d", time.hour())); // LCD显示,不足两位数补0
    ui->lcdNumberMinute->display(QString::asprintf("%02d", time.minute()));
    ui->lcdNumberSecond->display(QString::asprintf("%02d", time.second()));
    int val = ui->progressBar->value();
    val++;
    if (val > 100) val = 0;
    ui->progressBar->setValue(val);
}

void TimeDateSetDialog::on_pushButtonTimeBegin_clicked()
{
    ui->lineEditElapse->clear();
    m_time->start(); //计时器开始
    ui->pushButtonTimeBegin->setEnabled(false);
    ui->pushButtonTimeStop->setEnabled(true);
    //    ui->pushButtonSetPeriod->setEnabled(false);
}

void TimeDateSetDialog::on_pushButtonTimeStop_clicked()
{
    int mSec = m_time->elapsed(); //计算计时时长
    ui->lineEditElapse->setText(QString::asprintf("计时 %d ms", mSec));
    //    m_timer->stop();
    ui->pushButtonTimeBegin->setEnabled(true);
    ui->pushButtonTimeStop->setEnabled(false);
    //    ui->pushButtonSetPeriod->setEnabled(true);
}

void TimeDateSetDialog::on_pushButtonSetPeriod_clicked()
{
    m_timer->setInterval(ui->spinBoxElapse->value());
}
