#include "Projectile.h"

Projectile::Projectile(int team, int speed, float damage, Vector2Int position, Vector2Int direction) 
	: FieldObject(team, speed, position, direction), damage(damage) {}

float Projectile::GetDamage()
{
	return damage;
}
