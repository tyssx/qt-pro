#include <iostream>

using namespace std;

class Father
{
public:
    static int getHouse();
    static void setHouse(int value);

private:
    static int house;
};

int Father::house;

class Son : public Father
{
public:
    Son(int _house) { house = _house; }
    int getHouse() { return house; }

private:
    static int house;
};

int Son::house;

int Father::getHouse()
{
    return house;
}

void Father::setHouse(int value)
{
    house = value;
}

int main()
{
    Father father;
    father.setHouse(5);

    Son son(18);
    cout << "house" << son.getHouse();

    return 0;
}
