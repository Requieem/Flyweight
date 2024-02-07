#include "include/FieldObject.h"

FieldObject::FieldObject(int teamColor, int speed, Vector2Int position, Vector2Int direction) 
	: isMoving(true), teamColor(teamColor), speed(speed), position(position), previousPosition(position), direction(direction), cave(nullptr) {}

int FieldObject::Speed() const { return speed; }
int FieldObject::TeamColor() const { return teamColor; }

bool FieldObject::IsMoving() const { return isMoving; }
bool FieldObject::RemoveOnCollision() const { return false; }
void FieldObject::SetMoving(bool moving) { isMoving = moving; }

Vector2Int FieldObject::Position() const { return position; }
Vector2Int FieldObject::Direction() const { return direction; }
Vector2Int FieldObject::PreviousPosition() const { return previousPosition; }

Vector2Int FieldObject::NextPosition(Vector2Int dir) const { return Vector2Int::Sum(position, dir); }
void FieldObject::SetPosition(Vector2Int newPosition) { position = newPosition; }
void FieldObject::SetDirection(Vector2Int newDirection) { direction = newDirection; }
void FieldObject::SetCave(std::shared_ptr<std::vector<std::vector<bool>>> cave) { this->cave = cave; }

bool FieldObject::WillCollideWithCave(Vector2Int dir) const
{
	if (dir == Vector2Int::Zero) return false;

	int x = position.x + dir.x;
	int y = position.y + dir.y;

	return WillGoOutOfBounds(dir) || (*cave)[y][x];
}

bool FieldObject::WillGoOutOfBounds(Vector2Int dir) const
{
	if (dir == Vector2Int::Zero) return false;

	int x = position.x + dir.x;
	int y = position.y + dir.y;
	int maxX = (*cave)[0].size();
	int maxY = cave->size();

	bool outOfBounds = x < 0 || y < 0 || x >= maxX || y >= maxY;
	return outOfBounds;
}

void FieldObject::Move(Vector2Int dir) {
	previousPosition = position;
	position.Move(dir);

	int previousX = previousPosition.x;
	int previousY = previousPosition.y;

	int x = position.x;
	int y = position.y;

	(*cave)[previousY][previousX] = false;
	(*cave)[y][x] = true;
}