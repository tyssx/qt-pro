#ifndef QPERSON_H
#define QPERSON_H

#include <QMap>
#include <QMetaMethod>
#include <QObject>

#define REGISTER_CLASS(className) \
    namespace \
    { \
    int foo = ((void)RegistFactory::registerClass<className>(), 0); \
    }
//逗号表达式，从左往右依次执行，最终foo = 0

class RegistFactory
{
public:
    static QMap<int, QMetaObject> &constructors()
    {
        static QMap<int, QMetaObject> instance;
        return instance;
    }

    template<typename T> static void registerClass()
    {
        constructors().insert(T::Type, T::staticMetaObject);
    }

    static QObject *createObject(int type, QString name, QObject *parent = nullptr)
    {
        if (!constructors().contains(type)) return nullptr;
        const QMetaObject &mo = constructors().value(type);
        return mo.newInstance(
            Q_ARG(QString, name), Q_ARG(QObject *, parent)); // Q_ARG转换为QGenericArgument类型
    }
};

class QPerson : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("author", "Tao")
    Q_CLASSINFO("company", "Cpy")
    Q_CLASSINFO("version", "1.0.0")
    //属性定义
    //定义一个类型为int，名称为age的属性，READ，WRITE设置属性的读取，NOTIFY设置一个信号
    Q_PROPERTY(int age READ age WRITE setAge NOTIFY ageChanged)
    // MEMBER指定一个成员变量与属性关联，属性可读可写
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(int score MEMBER m_score)

public:
    Q_INVOKABLE explicit QPerson(QString fName, QObject *parent = nullptr); //反射给元对象构造函数
    Q_INVOKABLE int age(); // Q_INVOKABLE 注册成员函数，能够被反射给元对象
    void setAge(int value);
    void incAge();
    virtual QString birth();

    enum
    {
        STU = 1,
        TEA
    };

signals:
    void ageChanged(int value);

public slots:

private:
    int m_age = 10;
    QString m_name;
    int m_score = 79;
};

#endif // QPERSON_H
