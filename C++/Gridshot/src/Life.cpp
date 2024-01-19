#include "Life.h"
#include "Life.h"

bool Life::IsDead() const { return currentHealth <= 0; };
void Life::ChangeHealth(float amount) { currentHealth = std::clamp(currentHealth + amount, 0.0f, startingHealth); }
float Life::Health() const { return currentHealth; }
Life::Life() : startingHealth(1), currentHealth(1) {};
Life::Life(float startingHealth) : startingHealth(startingHealth), currentHealth(startingHealth) {};
