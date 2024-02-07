#include "include/Projectile.h"

Projectile::Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction) 
	: FieldObject(teamColor, speed, position, direction), Damage(damage) {}

bool Projectile::RemoveOnCollision() const { return true; }
Vector2Int Projectile::NextDirection() { return direction; }
std::string Projectile::DisplayChar() const { return "\xe2\x96\xa0"; }
