#pragma once

#include "FieldObject.h"


class Soldier : public FieldObject
{
	public:

		Soldier(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction);
		
		Vector2Int NextDirection() override;
		std::string DisplayChar() const override;
};

