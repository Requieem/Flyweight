#include "Vector2Int.h"

Vector2Int::Vector2Int() : x(0), y(0) {}
Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}

Vector2Int Vector2Int::Sum(Vector2Int vec1, Vector2Int vec2)
{
	return Vector2Int(vec1.x + vec2.x, vec1.y + vec2.y);
}

bool Vector2Int::operator<(const Vector2Int& other) const
{
	return (x < other.x) || (x == other.x && y < other.y);
}

Vector2Int Vector2Int::operator-() const
{
	return *new Vector2Int(-x, -y);
}

Vector2Int Vector2Int::operator+(Vector2Int* other) const
{
	return *new Vector2Int(x + other->x, y + other->y);
}

void Vector2Int::Move(Vector2Int direction)
{
	x += direction.x;
	y += direction.y;
}

const Vector2Int Vector2Int::Up = { 0, 1 };
const Vector2Int Vector2Int::Right = { 1, 0 };
