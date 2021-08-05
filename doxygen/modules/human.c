#include "human.h"

#include <stdlib.h>
#include <stdio.h>

struct human
{
    char *name; ///< Name of the human.
    int age;    ///< Age of the human.
};

Human human_create(char *name, int age)
{
    Human human = malloc(sizeof(*human));
    if (human == NULL)
    {
        return NULL;
    }

    human->name = name;
    human->age = age;

    return human;
}

void human_eat(Human human, char *food)
{
    printf("%s is about to eat %s ...");
    sleep(1);
    printf("finished!\n");
}

int human_age(Human human)
{
    return human->age;
}

void human_eternal_sleep(Human human)
{
    free(human->name);
    free(human);
}