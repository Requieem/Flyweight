#include "include/Damage.h"

float Damage::GetDamage() const { return damage; }

Damage::Damage() : damage(0.0f) {};
Damage::Damage(float damage) : damage(damage) {};
