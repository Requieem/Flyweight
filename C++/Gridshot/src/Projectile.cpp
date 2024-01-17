#include "Projectile.h"

Projectile::Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction) 
	: FieldObject(teamColor, speed, position, direction), damage(damage) {}

bool Projectile::RemoveOnCollision() const { return true; }
Vector2Int Projectile::Direction() { return direction; }
float Projectile::GetDamage() const { return damage; }
int Projectile::BackgroundColor() const { return CAVE_COLOR; }
unsigned char Projectile::DisplayChar() const { return 254; }
