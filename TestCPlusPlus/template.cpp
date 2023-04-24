#include "vld.h"

#include <iostream>

using namespace std;

template<class T> void func(T &a, T &b)
{
    T temp;
    temp = a;
    a    = b;
    b    = temp;
    cout << "a=" << a << "\tb=" << b << endl;
}

template<typename T> class Compare
{
public:
    Compare(T a, T b)
    {
        m_a = a;
        m_b = b;
    }

    T isMax() { return (m_a > m_b) ? m_a : m_b; }

private:
    T m_a;
    T m_b;
};

struct Stu
{
    int age;
    char *name;

    Stu operator=(Stu &stu)
    {
        age  = stu.age;
        name = new char[strlen(stu.name) + 1];
        strcpy(name, stu.name);
        return *this;
    }

    Stu(const Stu &stu)
    {
        age  = stu.age;
        name = new char[strlen(stu.name) + 1];
        strcpy(name, stu.name);
    }
};

struct stu
{
    int i;
    char c;
    char *p;
};

/*
int main()
{
    int a = 10;
    int b = 20;
    func(a, b);

    Compare<int> compare(a, b);
    cout << compare.isMax() << endl;

    Stu st1, st2;
    char *nm = (char *)"lishao";
    st1.age  = 15;
    st1.name = nm;

    st2 = st1;
    Stu st3(st2);

    cout << "st2\t" << st2.age << "\t" << st2.name << endl;
    //    cout << "st2 st1\t" << st2.name << "\t" << st1.name << endl;
    printf("s1 ptr: %p, s2 ptr : %p, s3 ptr : %p\n", st1.name, st2.name, st3.name);

    delete[] st2.name;

    struct stu s1, s2;

    char *str = (char *)"rabbit iscute ";
    s1.i      = 345;
    s1.c      = 'y';
    s1.p      = (char *)str;
    s2        = s1;
    printf("s2 %d, %c, %s\n", s2.i, s2.c, s1.p);
    printf("s1 ptr: %p, s2 ptr : %p\n", s1.p, s2.p);

    return 0;
}
*/
