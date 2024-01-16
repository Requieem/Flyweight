#pragma once
struct Vector2Int
{
	int x;
	int y;

	Vector2Int();
	Vector2Int(int x, int y);
	void Move(Vector2Int direction);
	static Vector2Int Sum(Vector2Int vec1, Vector2Int vec2);
	static int Distance(Vector2Int vec1, Vector2Int vec2);

	bool operator<(const Vector2Int& other) const;

	static Vector2Int const Up;
	static Vector2Int const Right;

	Vector2Int operator-() const;
	Vector2Int operator+(Vector2Int* other) const;
	Vector2Int operator+(Vector2Int other) const;
	Vector2Int operator/(int scalar) const;
	Vector2Int operator/(Vector2Int* other) const;
};

