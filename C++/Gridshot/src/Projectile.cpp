#include "Projectile.h"

Projectile::Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction) 
	: FieldObject(teamColor, speed, position, direction), damage(damage) {}

bool Projectile::RemoveOnCollision() const { return true; }
float Projectile::GetDamage() const { return damage; }
int Projectile::BackgroundColor() const { return CAVE_COLOR; }
char Projectile::DisplayChar() const { return 254; }
