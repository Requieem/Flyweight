#pragma once
#include "FieldObject.h"

class Projectile : public FieldObject
{
	private:

		float damage;
		
	public:

		Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction);
		
		float GetDamage() const;
		char DisplayChar() const override;
		int BackgroundColor() const override;
		bool RemoveOnCollision() const override;
};

