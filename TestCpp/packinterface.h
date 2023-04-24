#ifndef PACKINTERFACE_H
#define PACKINTERFACE_H

#include <iostream>

using namespace std;

class Packing
{
public:
    Packing() {}
    virtual ~Packing() {}
    virtual string pack() = 0;
};

class Wrapper : public Packing //包装纸
{
    string pack() { return "Wrapper"; }
};

class Bottle : public Packing //瓶子
{
public:
    string pack() { return "bottle"; }
};

#endif // PACKINTERFACE_H
