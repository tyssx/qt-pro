#include "widget.h"

#include "qstudent.h"
#include "qteacher.h"
#include "ui_widget.h"

#include <QDebug>
#include <QMetaClassInfo>
#include <QMetaProperty>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    boy = new QPerson("xiaoming");
    boy->setProperty("age", 17);
    boy->setProperty("score", 95);
    boy->setProperty("sex", "man"); //动态属性
    connect(boy, &QPerson::ageChanged, this, &Widget::slot_ageChanged);

    girl = new QPerson("xiaoli");
    girl->setProperty("age", 18);
    girl->setProperty("score", 98);
    girl->setProperty("sex", "woman"); //动态属性
    connect(girl, &QPerson::ageChanged, this, &Widget::slot_ageChanged);

    ui->spinBoxBoyAge->setProperty("isBoy", true); //动态属性
    ui->spinBoxGirlAge->setProperty("isBoy", false);

    connect(ui->spinBoxBoyAge, SIGNAL(valueChanged(int)), this, SLOT(slot_valueChanged(int)));
    connect(ui->spinBoxGirlAge, SIGNAL(valueChanged(int)), this, SLOT(slot_valueChanged(int)));
}

Widget::~Widget()
{
    delete ui;
    delete boy;
    delete girl;
}

void Widget::on_pushButtonBoyAdd_clicked()
{
    int num = ui->spinBoxBoyAge->value();
    ui->spinBoxBoyAge->setValue(++num);
}

void Widget::on_pushButtonGirlAdd_clicked()
{
    int num = ui->spinBoxGirlAge->value();
    ui->spinBoxGirlAge->setValue(++num);
}

void Widget::on_pushButtonMetaInfo_clicked()
{
#if 1
    //元对象信息
    const QMetaObject *meta = boy->metaObject();
    ui->textEdit->clear();
    ui->textEdit->append("==元对象信息==\n");
    ui->textEdit->append(QString("类名称: %1\n").arg(meta->className()));
    ui->textEdit->append("property属性");
    for (int i = meta->propertyOffset(); i < meta->propertyCount(); i++)
    {
        QMetaProperty prop   = meta->property(i);
        const char *propName = prop.name();
        QString propValue    = boy->property(propName).toString();
        ui->textEdit->append(QString("属性名称=%1,属性值=%2").arg(propName).arg(propValue));
    }
    ui->textEdit->append("");
    ui->textEdit->append("ClassInfo类信息");
    for (int j = meta->classInfoOffset(); j < meta->classInfoCount(); j++)
    {
        QMetaClassInfo classInfo = meta->classInfo(j);
        const char *className    = classInfo.name();
        QString classValue       = classInfo.value();
        ui->textEdit->append(QString("Name=%1,Value=%2").arg(className).arg(classValue));
    }
    ui->textEdit->append("");
    ui->textEdit->append("Method成员函数");
    qDebug() << "method count = " << meta->methodCount();
    for (int m = meta->methodOffset(); m < meta->methodCount(); m++)
    {
        QMetaMethod method = meta->method(m);
        QByteArray byteA   = method.name();
        ui->textEdit->append(QString("函数成员=%1").arg(QString(byteA)));
    }
    ui->textEdit->append("");
    ui->textEdit->append("Constructor构造函数");
    qDebug() << "constructor count = " << meta->constructorCount();
    for (int n = 0; n < meta->constructorCount(); n++)
    {
        QMetaMethod method = meta->constructor(n);
        QByteArray byteA   = method.name();
        ui->textEdit->append(QString("构造函数=%1").arg(QString(byteA)));
    }
#else
    const QMetaObject meta = QPerson::staticMetaObject;
    ui->textEdit->clear();
    ui->textEdit->append("==元对象信息==\n");
    ui->textEdit->append(QString("类名称: %1\n").arg(meta.className()));
    ui->textEdit->append("property属性");
    for (int i = meta.propertyOffset(); i < meta.propertyCount(); i++)
    {
        QMetaProperty prop   = meta.property(i);
        const char *propName = prop.name();
        QString propValue    = boy->property(propName).toString();
        ui->textEdit->append(QString("属性名称=%1,属性值=%2").arg(propName).arg(propValue));
    }
    ui->textEdit->append("");
    ui->textEdit->append("ClassInfo类信息");
    for (int j = meta.classInfoOffset(); j < meta.classInfoCount(); j++)
    {
        QMetaClassInfo classInfo = meta.classInfo(j);
        const char *className    = classInfo.name();
        QString classValue       = classInfo.value();
        ui->textEdit->append(QString("Name=%1,Value=%2").arg(className).arg(classValue));
    }
#endif
}

void Widget::on_pushButtonClearText_clicked()
{
    ui->textEdit->clear();
}

void Widget::slot_ageChanged(int value)
{
    Q_UNUSED(value);
    QPerson *person = qobject_cast<QPerson *>(sender());
    QString hisName = person->property("name").toString(); //姓名
    QString hisSex  = person->property("sex").toString();  //性别
    int hisAge      = person->age();                       //通过接口获取年龄
    //    int hisAge = person->property("age").toInt();    //通过属性获取年龄
    ui->textEdit->append(
        "姓名：" + hisName + ",性别：" + hisSex + QString::asprintf(",年龄：%d", hisAge));
}

void Widget::slot_valueChanged(int value)
{
    Q_UNUSED(value);
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());
    if (spinBox->property("isBoy").toBool())
        boy->setAge(spinBox->value());
    else
    {
        girl->setAge(spinBox->value());
    }
}

void Widget::on_pushButtonResStu_clicked()
{
    //注册类
    //    RegistFactory::registerClass<QStudent>();
    QObject *obj  = RegistFactory::createObject(QStudent::Type, "lihua");
    QPerson *pers = qobject_cast<QPerson *>(obj);

    //设置属性
    pers->setProperty("age", 20);
    pers->setProperty("sex", "man");

    //获取属性
    QString hisName = pers->property("name").toString(); //姓名
    QString hisSex  = pers->property("sex").toString();  //性别
    int hisAge      = pers->age();                       //通过接口获取年龄
    ui->textEdit->append("");
    ui->textEdit->append(
        "姓名：" + hisName + ",性别：" + hisSex + QString::asprintf(",年龄：%d", hisAge));
    ui->textEdit->append("出生：" + pers->birth());
}

void Widget::on_pushButtonRegTea_clicked()
{
    //注册类
    //    RegistFactory::registerClass<QStudent>();
    QObject *obj  = RegistFactory::createObject(QTeacher::Type, "zhanglaoshi");
    QPerson *pers = qobject_cast<QPerson *>(obj);

    //设置属性
    pers->setProperty("age", 40);
    pers->setProperty("sex", "man");

    //获取属性
    QString hisName = pers->property("name").toString(); //姓名
    QString hisSex  = pers->property("sex").toString();  //性别
    int hisAge      = pers->age();                       //通过接口获取年龄
    ui->textEdit->append("");
    ui->textEdit->append(
        "姓名：" + hisName + ",性别：" + hisSex + QString::asprintf(",年龄：%d", hisAge));
    ui->textEdit->append("出生：" + pers->birth());
}
