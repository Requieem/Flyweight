#pragma once
#include "FieldObject.h"
class Soldier : public FieldObject
{
	private:

		float startingHealth;
		float currentHealth;

	public:

		Soldier(int team, int speed, float startingHealth, Vector2Int position, Vector2Int direction);
		float GetHealth();
		bool ChangeHealth(float change);
		bool IsDead();
};

