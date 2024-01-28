#pragma once

#include <vector>
#include <stdexcept>

struct Vector2Int
{
	int x;
	int y;

	Vector2Int();
	Vector2Int(int x, int y);
	void Move(Vector2Int direction);
	static Vector2Int Sum(Vector2Int vec1, Vector2Int vec2);
	static int Distance(Vector2Int vec1, Vector2Int vec2);

	static Vector2Int const Uno;
	static Vector2Int const Zero;

	static Vector2Int const Up;
	static Vector2Int const Down;
	static Vector2Int const Left;
	static Vector2Int const Right;

	static Vector2Int const UpLeft;
	static Vector2Int const DownLeft;
	static Vector2Int const UpRight;
	static Vector2Int const DownRight;

	static std::vector<Vector2Int> const Directions;

	Vector2Int operator-() const;
	Vector2Int operator-(Vector2Int other) const;
	Vector2Int operator/(int scalar) const;
	Vector2Int operator+(Vector2Int other) const;
	Vector2Int operator/(Vector2Int other) const;
	bool operator==(Vector2Int other) const;
	bool operator<(const Vector2Int other) const;
	int ManhattanDistance(const Vector2Int other) const;
};

struct Vector2IntHash {
	std::size_t operator()(const Vector2Int& v) const noexcept;
};
