#pragma once

#include <map>
#include <vector>
#include <memory>

#include "Life.h"
#include "Colors.h"
#include "Damage.h"
#include "Vector2Int.h"
class FieldObject : virtual public Life, virtual public Damage

{
protected:
	int speed;
	int teamColor;
	bool isMoving;

	Vector2Int position;
	Vector2Int direction;
	Vector2Int previousPosition;
	std::shared_ptr<std::vector<std::vector<bool>>> cave;

	FieldObject(int teamColor, int speed, Vector2Int position, Vector2Int direction);

public:
	int Speed() const;
	int TeamColor() const;
	bool IsMoving() const;
	virtual bool RemoveOnCollision() const;

	virtual Vector2Int NextDirection() = 0;
	virtual unsigned char DisplayChar() const = 0;

	Vector2Int Position() const;
	Vector2Int Direction() const;
	Vector2Int NextPosition(Vector2Int dir) const;
	Vector2Int PreviousPosition() const;

	void Move(Vector2Int dir);
	void SetMoving(bool moving);
	void SetPosition(Vector2Int newPosition);
	void SetDirection(Vector2Int newDirection);
	void SetCave(std::shared_ptr<std::vector<std::vector<bool>>> field);
	bool WillCollideWithCave(Vector2Int dir) const;
	bool WillGoOutOfBounds(Vector2Int dir) const;
};
