#pragma once
#include "Vector2Int.h"
#include "FieldObject.h"

class Projectile : public FieldObject
{
	private:

		float damage;
		
	public:

		Projectile(int team, int speed, float damage, Vector2Int position, Vector2Int direction);
		float GetDamage();
};

