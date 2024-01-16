#include "Soldier.h"

Soldier::Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction) 
    : FieldObject(teamColor, speed, position, direction), startingHealth(startingHealth), currentHealth(startingHealth) {}

char Soldier::DisplayChar() const { return 219; }
float Soldier::Health() const { return currentHealth; }
bool Soldier::IsDead() const { return currentHealth <= 0; }
int Soldier::BackgroundColor() const { return CAVE_COLOR; }

bool Soldier::ChangeHealth(float change)
{
    currentHealth += change;
    return IsDead();
}