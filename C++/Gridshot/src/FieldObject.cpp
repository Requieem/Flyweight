#include "FieldObject.h"

FieldObject::FieldObject(int team, int speed, Vector2Int position, Vector2Int direction) : isMoving(true),
	team(team), speed(speed), position(position), previousPosition(position), direction(direction) {}

int FieldObject::GetSpeed()
{
	return speed;
}

int FieldObject::GetTeam()
{
	return team;
}

bool FieldObject::IsMoving()
{
	return isMoving;
}

void FieldObject::SetMoving(bool moving)
{
	isMoving = moving;
}

Vector2Int FieldObject::Position()
{
	return position;
}

Vector2Int FieldObject::PreviousPosition()
{
	return previousPosition;
}

void FieldObject::Move()
{
	previousPosition = position;
	position.Move(direction);
}

void FieldObject::SetPosition(Vector2Int newPosition)
{
	position = newPosition;
}

void FieldObject::SetDirection(Vector2Int newDirection)
{
	direction = newDirection;
}

Vector2Int FieldObject::Direction()
{
	return direction;
}

Vector2Int FieldObject::NextPosition()
{
	return Vector2Int::Sum(position, direction);
}
