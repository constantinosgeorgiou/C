/// @file human.h
///
/// Interface for human module.
///
/// Operations included:
/// - create: Creates human.
/// - eternal_sleep: Destroys human.
/// - eat: Instructs a human to eat food.
/// - age: Returns the age of a human.

#pragma once

typedef struct human *Human; ///< Human type.

/// Creates a human with NAME and AGE.
///
/// Every other aspect of what defines a human is randomized.
///
/// @note No saucy stuff take place during the creation.
///
/// @param Name of human.
/// @param Age of human.
///
/// @return Newly created human.
///
Human human_create(char *name, int age);

/// Instruct HUMAN to eat FOOD.
///
/// If modern HUMAN, use fork and knife, or chopsticks.
///
/// @param food Needs to be something edible.
///
void human_eat(Human human, char *food);

/// Returns the age of HUMAN.
///
int human_age(Human human);

/// Puts HUMAN to eternal sleep.
///
/// Nicer way of saying that the human dies. Memory freed.
///
void human_eternal_sleep(Human human);