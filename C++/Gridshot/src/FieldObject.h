#pragma once

#include <map>
#include <vector>
#include <memory>

#include "Colors.h"
#include "Vector2Int.h"

class FieldObject
{
private:
	int teamColor;
	int speed;
	bool isMoving;

	Vector2Int previousPosition;
	Vector2Int position;
	Vector2Int direction;

protected:
	FieldObject(int teamColor, int speed, Vector2Int position, Vector2Int direction);

public:
	int Speed() const;
	int TeamColor() const;
	bool IsMoving() const;
	virtual bool RemoveOnCollision() const;
	virtual char DisplayChar() const = 0;
	virtual int BackgroundColor() const = 0;

	Vector2Int Position() const;
	Vector2Int Direction() const;
	Vector2Int NextPosition() const;
	Vector2Int PreviousPosition() const;

	void Move();
	void SetMoving(bool moving);
	void SetPosition(Vector2Int newPosition);
	void SetDirection(Vector2Int newDirection);

	friend bool operator==(const std::shared_ptr<FieldObject>& lhs, const std::shared_ptr<FieldObject>& rhs) {
		return lhs.get() == rhs.get();
	}
};
