#include <iostream>

using namespace std;

class Person
{
public:
    Person(int age)
    {
        m_age = age;
        cout << "Person construct ...\t age is " << m_age << endl;
    }
    ~Person() { cout << "Person destroy ...\t age is " << m_age << endl; }
    int age() { return m_age; }

private:
    int m_age;
};

//仿函数，内存删除
class DestroyPerson
{
public:
    void operator()(Person *person)
    {
        cout << "person will destroy ...\t age is " << person->age() << " " << endl;
    }
};

/*
int main()
{
    shared_ptr<Person> p1;
    Person *person1 = new Person(24);
    p1.reset(person1); // reset方式初始化

    cout << __LINE__ << " get raw " << p1.get() << endl;

    shared_ptr<Person> p2(new Person(32)); //构造函数初始化

    shared_ptr<Person> p3(new Person(35), DestroyPerson()); //带删除

    shared_ptr<Person> p4 = make_shared<Person>(47); //通过std::make_shared<T>初始化，C++推荐
    //    shared_ptr<Person> p5 = make_shared<Person, DestroyPerson()>(12);
    //    //不允许make_shared带删除器

    shared_ptr<Person> p6 = p1;
    cout << __LINE__ << " p6 use_count " << p6.use_count() << endl; // result 2
    shared_ptr<Person> p7(p1);
    cout << __LINE__ << " p7 use_count " << p7.use_count() << endl; // result 3

    p7 = nullptr;
    cout << __LINE__ << " p7 use_count " << p7.use_count() << endl; // result 0

    swap(p2, p3);

    return 0;
}*/
