#pragma once

#include <random>
#include <chrono>
#include "AStar.h"
#include "Soldier.h"

class Enemy : public Soldier
{
private:

	float tickRate;
	std::vector<Vector2Int> directionPath;
	std::shared_ptr<FieldObject> targetObject;
	std::vector<Vector2Int>::iterator pathIterator;
	std::chrono::steady_clock::time_point lastDecisionTime;
	Vector2Int RandomDirection() const;

public:

	Enemy(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction, std::shared_ptr<FieldObject> targetObject);
	Vector2Int NextDirection() override;
};

