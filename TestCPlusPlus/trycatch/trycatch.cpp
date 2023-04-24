#include <iostream>

using namespace std;

double divide(double a, double b)
{
    const double delta = 0.000000001;
    double ret         = 0;

    if (!(-delta < b) && (b < delta)) { ret = a / b; }
    else
    {
        throw "Divided by zero";
    }

    cout << "divide(double a, double b)" << endl;

    return ret;
}

double ExceptionFunc()
{
    double d = divide(2, 0);

    cout << "ExceptionFunc()" << endl;

    return d;
}
/*
int main()
{
    double d = ExceptionFunc();
    cout << "result = " << d << endl;
    return 0;
}
*/
