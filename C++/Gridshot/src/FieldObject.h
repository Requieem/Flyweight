#pragma once
#ifndef FIELDOBJECT_H
#define FIELDOBJECT_H

#include "Vector2Int.h"
#include <vector>
#include <memory>
#include <map>

class FieldObject
{
	private:
		int team;
		int speed;
		bool isMoving;

		Vector2Int previousPosition;
		Vector2Int position;
		Vector2Int direction;

	protected:
		FieldObject(int team, int speed, Vector2Int position, Vector2Int direction);
		virtual ~FieldObject() = default;

	public:

		int GetTeam();
		int GetSpeed();
		bool IsMoving();

		Vector2Int Position();
		Vector2Int PreviousPosition();
		Vector2Int Direction();
		Vector2Int NextPosition();

		void Move();
		void SetMoving(bool moving);
		void SetPosition(Vector2Int newPosition);
		void SetDirection(Vector2Int newDirection);

		friend bool operator==(const std::shared_ptr<FieldObject>& lhs, const std::shared_ptr<FieldObject>& rhs) {
			return lhs.get() == rhs.get();
		}
};

typedef std::vector<std::shared_ptr<FieldObject>> FieldObjectList;
typedef std::map<Vector2Int, std::vector<std::shared_ptr<FieldObject>>> FieldObjectMap;
using SharedFieldObject = std::shared_ptr<FieldObject>;


#endif

