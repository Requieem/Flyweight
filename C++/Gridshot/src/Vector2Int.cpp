#include "Vector2Int.h"

Vector2Int::Vector2Int() : x(0), y(0) {}
Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}

const Vector2Int Vector2Int::Up = { 0, 1 };
const Vector2Int Vector2Int::Right = { 1, 0 };

Vector2Int Vector2Int::Sum(Vector2Int vec1, Vector2Int vec2)
{
	return { vec1.x + vec2.x, vec1.y + vec2.y };
}

int Vector2Int::Distance(Vector2Int vec1, Vector2Int vec2)
{
	int dx = vec1.x - vec2.x;
	int dy = vec1.y - vec2.y;

	int distance = static_cast<int>(sqrt(pow(dx, 2) + pow(dy, 2)) + 0.5);
	return distance;
}

bool Vector2Int::operator<(const Vector2Int other) const
{
	return (x < other.x) || (x == other.x && y < other.y);
}

int Vector2Int::ManhattanDistance(const Vector2Int other) const
{
	return std::abs(x - other.x) + std::abs(y - other.y);
}

Vector2Int Vector2Int::operator-() const
{
	return { -x, -y };
}

Vector2Int Vector2Int::operator-(Vector2Int other) const
{
	return { x - other.x, y - other.y };
}

Vector2Int Vector2Int::operator+(Vector2Int other) const
{
	return { x + other.x, y + other.y };
}

Vector2Int Vector2Int::operator/(int scalar) const
{
	if (scalar == 0)
		throw std::runtime_error("Division by 0 on a Vector2Int/scalar. Check that the scalar is not 0.");
	return *new Vector2Int(x/scalar, y/scalar);
}

Vector2Int Vector2Int::operator/(Vector2Int other) const
{
	if (other.x == 0 || other.y == 0)
		throw std::runtime_error("Division by 0 on a Vector2Int/Vecto2Int. Check that the divisor does not have 0 values.");
	return { x / other.x, y / other.y };
}

bool Vector2Int::operator==(Vector2Int other) const
{
	return x == other.x && y == other.y;
}

void Vector2Int::Move(Vector2Int direction)
{
	x += direction.x;
	y += direction.y;
}

std::size_t Vector2IntHash::operator()(const Vector2Int& v) const noexcept
{
	return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1);
}
