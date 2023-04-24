#include "widget.h"

#include "ui_widget.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::connectBase()
{
    QString baseName = "test";
    db               = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("127.0.0");
    db.setDatabaseName(baseName); //
    db.setUserName("user");
    db.setPassword("pwssword");
    if (!db.open()) { qDebug() << "open error" << db.lastError(); }
}

void Widget::insertData()
{
    QSqlQuery query(db);
    QString cmd = "insert into notes(title,tag,time,content) values('QSlite "
                  "test','database','2021-01-21','how to user  QSlite');";
    if (!query.exec(cmd)) { qDebug() << "insert error"; }
}
