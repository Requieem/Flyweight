#pragma once

#include "AStar.h"
#include "Soldier.h"

class Enemy : public Soldier
{
private:

	std::shared_ptr<FieldObject> targetObject;
	std::vector<Vector2Int> directionPath;
	std::vector<Vector2Int>::iterator pathIterator;

public:

	Enemy(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction, std::shared_ptr<FieldObject> targetObject);
	Vector2Int Direction() override;
};

