#include "Life.h"
#include "Life.h"

bool Life::IsDead() const { return currentHealth <= 0; };
void Life::ChangeHealth(float amount) { currentHealth += amount; };

Life::Life() : startingHealth(1), currentHealth(1) {};
Life::Life(float startingHealth) : startingHealth(startingHealth), currentHealth(startingHealth) {};
