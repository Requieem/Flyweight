#pragma once

#include <map>
#include <vector>
#include <memory>

#include "Colors.h"
#include "Vector2Int.h"

class FieldObject
{
protected:
	int teamColor;
	int speed;
	bool isMoving;

	std::shared_ptr<std::vector<std::vector<bool>>> cave;
	Vector2Int position;
	Vector2Int direction;
	Vector2Int previousPosition;

protected:
	FieldObject(int teamColor, int speed, Vector2Int position, Vector2Int direction);

public:
	int Speed() const;
	int TeamColor() const;
	bool IsMoving() const;
	virtual bool RemoveOnCollision() const;
	virtual unsigned char DisplayChar() const = 0;
	virtual int BackgroundColor() const = 0;

	Vector2Int Position() const;
	virtual Vector2Int Direction() = 0;
	Vector2Int NextPosition() const;
	Vector2Int PreviousPosition() const;

	void Move();
	void SetCave(std::shared_ptr<std::vector<std::vector<bool>>> field);
	void SetMoving(bool moving);
	void SetPosition(Vector2Int newPosition);
	void SetDirection(Vector2Int newDirection);
	bool WillCollideWithCave(Vector2Int dir) const;

	friend bool operator==(const std::shared_ptr<FieldObject>& lhs, const std::shared_ptr<FieldObject>& rhs) {
		return lhs.get() == rhs.get();
	}
};
