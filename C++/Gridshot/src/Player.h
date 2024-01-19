#pragma once

#include "Score.h"
#include "Soldier.h"

class Player : public Soldier, public Score
{
public:
	Player(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction);
};

