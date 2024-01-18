#pragma once
#include "FieldObject.h"

class Soldier : public FieldObject
{
	public:

		Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction);
		
		bool IsDead() const;
		float Health() const;
		Vector2Int NextDirection() override;
		unsigned char DisplayChar() const override;
		int BackgroundColor() const override;
};

