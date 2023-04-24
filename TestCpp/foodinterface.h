#ifndef FOODINTERFACE_H
#define FOODINTERFACE_H

#include "packinterface.h"

#include <string>

class Food
{
public:
    Food() {}
    virtual ~Food() {}
    virtual Packing *getPack() = 0;
    virtual string getName()   = 0;
    virtual float getPrice()   = 0;
};

class Burger : public Food
{
public:
    Packing *getPack() { return new Wrapper(); }
    virtual string getName() = 0;
    virtual float getPrice() = 0;
};

class ColdDrink : public Food

{
public:
    Packing *getPack() { return new Bottle(); }
    virtual string getName() = 0;
    virtual float getPrice() = 0;
};

class VegBurger : public Burger

{
public:
    string getName() { return "veg burger"; }
    float getPrice() { return 25.0f; }
};

class ChickenBurger : public Burger
{
public:
    string getName() { return "chicken Burger"; }
    float getPrice() { return 50.5f; }
};

class Coke : public ColdDrink
{
public:
    string getName() { return "coke"; }
    float getPrice() { return 30.0f; }
};

class Pepsi : public ColdDrink
{
public:
    string getName() { return "coke"; }
    float getPrice() { return 35.0f; }
};

#endif // FOODINTERFACE_H
