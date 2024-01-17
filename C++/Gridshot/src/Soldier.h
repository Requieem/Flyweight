#pragma once
#include "FieldObject.h"

class Soldier : public FieldObject
{
	private:

		float startingHealth;
		float currentHealth;

	public:

		Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction);
		
		bool IsDead() const;
		float Health() const;
		bool ChangeHealth(float change);
		Vector2Int Direction() override;
		unsigned char DisplayChar() const override;
		int BackgroundColor() const override;
};

