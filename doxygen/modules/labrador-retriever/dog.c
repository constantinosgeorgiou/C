/// @file
///
/// Implementation of dog interface.
///
/// @implements Dog

#include "dog.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct dog
{
    char *name;          ///< Name of the dog.
    int age;             ///< Age of the dog.
    int balls_retrieved; ///< Number of successfuly retrieved balls.
    char *breed;         ///< Breed of the dog.
};

Dog dog_wake(char *name, int age)
{
    Dog dog = malloc(sizeof(*dog));
    if (dog == NULL)
    {
        return NULL;
    }

    dog->name = name;
    dog->age = age;
    dog->breed = strdup("labrador-retriever");

    return dog;
}

char *dog_breed(Dog dog)
{
    return dog->breed;
}

void dog_bark_at(Dog dog, char *something)
{
    printf("Woof woof woof! %s barked at %s happily\n", dog->name, something);
}

/// @details Complexity: O(n^2)
///
bool dog_fetch_ball(Dog dog)
{
    if (rand() % 2)
    {
        // Heads
        dog->balls_retrieved++;
        return true;
    }
    else
    {
        // Tails
        return false;
    }
}

int dog_balls_fetched(Dog dog)
{
    return dog->balls_retrieved;
}

void dog_sleep(Dog dog)
{
    free(dog->name);
    free(dog->breed);
    free(dog);
}