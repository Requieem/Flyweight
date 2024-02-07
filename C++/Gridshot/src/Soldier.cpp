#include "include/Soldier.h"

Soldier::Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction)
    : FieldObject(teamColor, speed, position, direction), Life(startingHealth) {}

std::string Soldier::DisplayChar() const { return "\u2588"; }
Vector2Int Soldier::NextDirection() { return direction; }