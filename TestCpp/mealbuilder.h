#ifndef MEALBUILDER_H
#define MEALBUILDER_H

#include "meal.h"

class MealBuilder
{
public:
    Meal *prepareVegMeal()
    {
        Meal *meal = new Meal();
        meal->addFoodToVector(new VegBurger());
        meal->addFoodToVector(new Coke());
        return meal;
    }

    Meal *prepareNonMeal()
    {
        Meal *meal = new Meal();
        meal->addFoodToVector(new ChickenBurger());
        meal->addFoodToVector(new Pepsi);
        return meal;
    }
};

#endif // MEALBUILDER_H
