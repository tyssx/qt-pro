#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QMovie>

class WaitDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WaitDialog(QWidget *parent, const QString &title);
    ~WaitDialog();

private:
    QMovie *movie;
    QLabel *label;
    QLabel *tip_label;
    QFrame *background;
};

#endif // WAITDIALOG_H
