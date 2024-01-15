#include "Soldier.h"

Soldier::Soldier(int team, int speed, float startingHealth, Vector2Int position, Vector2Int direction) 
    : FieldObject(team, speed, position, direction), startingHealth(startingHealth), currentHealth(startingHealth) {}

float Soldier::GetHealth()
{
    return currentHealth;
}

bool Soldier::ChangeHealth(float change)
{
    currentHealth += change;
    return IsDead();
}

bool Soldier::IsDead()
{
    return currentHealth <= 0;
}
