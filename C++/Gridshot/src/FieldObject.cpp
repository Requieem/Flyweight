#include "FieldObject.h"

FieldObject::FieldObject(int teamColor, int speed, Vector2Int position, Vector2Int direction) 
	: isMoving(true), teamColor(teamColor), speed(speed), position(position), previousPosition(position), direction(direction) {}

int FieldObject::Speed() const { return speed; }
int FieldObject::TeamColor() const { return teamColor; }

bool FieldObject::IsMoving() const { return isMoving; }
bool FieldObject::RemoveOnCollision() const { return false; }
void FieldObject::SetMoving(bool moving) { isMoving = moving; }

Vector2Int FieldObject::Position() const { return position; }
Vector2Int FieldObject::PreviousPosition() const { return previousPosition; }
Vector2Int FieldObject::NextPosition() const { return Vector2Int::Sum(position, direction); }
void FieldObject::SetPosition(Vector2Int newPosition) { position = newPosition; }

Vector2Int FieldObject::Direction() const { return direction; }
void FieldObject::SetDirection(Vector2Int newDirection) { direction = newDirection; }

void FieldObject::Move() {
	previousPosition = position;
	position.Move(direction);
}
