/// @file dog.h
///
/// Interface for Dog module.
///
/// This module supports the following operations:
/// - wake: Creation of the structure.
/// - sleep: Destruction of the structure.
/// - bark_at: Instruct the dog to bark at something.
/// - fetch_ball: Instruct the dog to fetch a ball.
/// - balls_fetched: Returns the number of balls fetched.

#pragma once

#include <stdbool.h> // bool

#define SUCCESS_RATE 0.5 ///< The success probability for a dog to fetch a ball.

typedef struct dog *Dog; ///< Dog type.

/// Creates a dog with NAME and AGE.
///
/// Age doesn't really matter, but it should be a reasonable number.
///
/// @param name Name of the dog. (Should not contain number or special characters)
/// @param age Age of the dog. (Should be a reasonable number)
///
/// @return Newly created dog.
///
Dog dog_wake(char *name, int age);

/// Returns the breed of DOG.
///
char *dog_breed(Dog dog);

/// Instructs the DOG to bark at SOMETHING.
///
/// Should not be annoying.
///
/// @note Notify the neighbors before executing this function.
///
/// @param something An object/person for the dog to bark at.
///
void dog_bark_at(Dog dog, char *something);

/// Instructs the DOG to fetch a ball.
///
/// @return true, if ball fetched successfully, or false, if ball was lost.
///
bool dog_fetch_ball(Dog dog);

/// Return the number of successfuly fetched balls by DOG.
///
int dog_balls_fetched(Dog dog);

/// Puts DOG to sleep.
///
/// This is a more subtle way to say that DOG gets destroyed, its memory freed.
///
/// @note 'F' for respect.
///
void dog_sleep(Dog dog);