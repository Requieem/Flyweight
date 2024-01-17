#include "Enemy.h"

Enemy::Enemy(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction, std::shared_ptr<FieldObject> targetObject) 
: Soldier(teamColor, speed, startingHealth, position, direction), targetObject(targetObject) {}

Vector2Int Enemy::Direction()
{
	if (directionPath.empty() || pathIterator != directionPath.end() - 1)
	{
		directionPath = AStar::DirectionPath(position, targetObject->Position(), cave);
		pathIterator = directionPath.begin();
	}

	if (pathIterator != directionPath.end())
	{
		return *pathIterator++;
	}
	else
	{
		return { 0 , 0 };
	}
}
