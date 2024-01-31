#include "../include/Enemy.h"

Enemy::Enemy(int teamColor, int speed, float startingHealth, Vector2Int position, Vector2Int direction, std::shared_ptr<FieldObject> targetObject)
	: Soldier(teamColor, speed, startingHealth, position, direction), Damage(1), targetObject(targetObject), tickRate(100), lastDecisionTime(std::chrono::steady_clock::now()) {}

Vector2Int Enemy::NextDirection()
{
	int distance = Vector2Int::Distance(position, targetObject->Position());
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastDecisionTime).count();

	if (elapsedTime <= tickRate)
	{
		direction = Vector2Int::Zero;
	}
	else
	{
		if (distance >= 25)
		{
			direction = RandomDirection();
		}
		else if (!directionPath.empty() && pathIterator != directionPath.end())
		{
			if (!WillCollideWithCave(*pathIterator))
				direction = *pathIterator++;
		}
		else
		{
			directionPath = AStar::DirectionPath(position, targetObject->Position(), cave);
			pathIterator = directionPath.begin();

			if (!directionPath.empty() && pathIterator != directionPath.end())
			{
				if (!WillCollideWithCave(*pathIterator))
					direction = *pathIterator++;
			}
		}

		if (direction == Vector2Int::Zero)
			direction = RandomDirection();

		lastDecisionTime = currentTime;
	}

	return direction;
}

Vector2Int Enemy::RandomDirection() const
{
	if (direction != Vector2Int::Zero && !WillCollideWithCave(direction))
		return direction;

	std::vector<Vector2Int> validDirections;
	Vector2Int randomDirection = Vector2Int::Zero;

	for (const auto& dir : Vector2Int::Directions) {
		if (!WillCollideWithCave(dir)) {
			validDirections.push_back(dir);
		}
	}

	if (!validDirections.empty()) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<size_t> distribution(0, validDirections.size() - 1);

		size_t randomIndex = distribution(gen);
		randomDirection = validDirections[randomIndex];
	}

	return randomDirection;
}