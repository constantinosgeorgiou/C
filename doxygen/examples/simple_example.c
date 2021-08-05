#include "human.h"
#include "dog.h"

int main(int argc, char const *argv[])
{
    Human human = human_create("Bob", 24);
    Dog dog = dog_wake("Coffee", 2);

    human_eternal_sleep(human);
    dog_sleep(dog);

    return 0;
}
