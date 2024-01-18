#include "Projectile.h"

Projectile::Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction) 
	: FieldObject(teamColor, speed, position, direction), Damage(damage) {}

bool Projectile::RemoveOnCollision() const { return true; }
Vector2Int Projectile::NextDirection() { return direction; }
int Projectile::BackgroundColor() const { return CAVE_COLOR; }
unsigned char Projectile::DisplayChar() const { return 254; }
