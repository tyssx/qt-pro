#include <QCoreApplication>
#include <iostream>

using namespace std;

class Test
{
public:
    Test() { cout << "Test construct..." << endl; }
    ~Test() { cout << "Test destroy..." << endl; }

    int getDebug() { return this->debug; }

private:
    int debug = 20;
};

struct DestroyTest
{
    void operator()(Test *test) { cout << "delete test " << test->getDebug() << " " << endl; }
};
/*
int main(int argc, char *argv[])
{
    auto_ptr<Test> test(new Test);

    cout << __FUNCTION__ << " " << __LINE__ << " " << test->getDebug() << endl;
    cout << __FUNCTION__ << " " << __LINE__ << " " << (*test).getDebug() << endl;

    auto_ptr<Test> p1(new Test);
    auto_ptr<Test> p2(new Test);

    p1 = p2;
    cout << __FUNCTION__ << " " << __LINE__ << " " << p1.get()
         << endl; //赋值后p1托管p2的内存，p2托管变成NULL
    cout << __FUNCTION__ << " " << __LINE__ << " " << p2.get() << endl;

    unique_ptr<Test> p3(new Test);
    unique_ptr<Test> p4(new Test);

    //    p3 = p4; //禁止左值赋值
    //    unique_ptr<Test> p5(p3); //禁止左值赋值构造

    unique_ptr<Test> p5(std::move(p3));
    p5 = std::move(p3); // 使用move把左值转成右值就可以赋值了，效果和auto_ptr赋值一样

    unique_ptr<Test, DestroyTest> p6(new Test);
    cout << __LINE__ << " " << p6.get() << endl;

    unique_ptr<Test> p7(new Test);
    p7 = nullptr;
    p7.reset();

    unique_ptr<Test> p8(new Test);
    Test *ptest = p8.release();

    unique_ptr<int[]> arr(new int[10]);
    arr[9] = 10;
    cout << __LINE__ << "arr" << arr[9] << endl;

    return 0;
}*/
