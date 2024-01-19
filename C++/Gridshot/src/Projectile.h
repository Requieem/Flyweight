#pragma once
#include "FieldObject.h"

class Projectile : public FieldObject
{
	public:

		Projectile(int teamColor, int speed, float damage, Vector2Int position, Vector2Int direction);
		
		Vector2Int NextDirection() override;
		unsigned char DisplayChar() const override;
		bool RemoveOnCollision() const override;
};

