#ifndef MEAL_H
#define MEAL_H

#include "foodinterface.h"

#include <vector>

class Meal
{
public:
    Meal() { vec.reserve(255); }
    ~Meal()
    {
        for (int i = 0; i < vec.size(); i++) {}
    }

    void addFoodToVector(Food *food) { vec.push_back(food); }
    float getCost()
    {
        float ret = 0.0;
        for (auto &food: vec) { ret += food->getPrice(); }
        return ret;
    }
    void showFoods()
    {
        for (auto &food: vec)
        {
            cout << "------------------------" << endl;
            cout << food->getName() << endl;
            cout << food->getPrice() << endl;
            cout << food->getPack() << endl;
        }
    }

public:
    std::vector<Food *> vec;
};

#endif // MEAL_H
