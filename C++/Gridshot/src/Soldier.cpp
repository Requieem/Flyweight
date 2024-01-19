#include "Soldier.h"

Soldier::Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction)
    : FieldObject(teamColor, speed, position, direction), Life(startingHealth) {}

unsigned char Soldier::DisplayChar() const { return 219; }
Vector2Int Soldier::NextDirection() { return direction; }