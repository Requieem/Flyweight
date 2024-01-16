#pragma once

#include <ctime>
#include <vector>
#include <cstdlib>

#include "Vector2Int.h"

typedef std::vector<std::vector<bool>> Cave;

class CaveGenerator
{
private:
	Cave cave;
	Cave minusCave;

	float fillProbability;

	Vector2Int size;
	Vector2IntList emptySpaces;

	void CaveIteration();
	void GenerateInitialCave();
	int CountSolidNeighbors(const int x, const int y) const;

public:
	CaveGenerator(Vector2Int size, float fillProbability);

	void GenerateCave(const int iterations, const Vector2Int* size, const float* fillProbability);

	Cave GetCave() const;
	Cave GetMinusCave() const;
	Vector2Int GetSize() const;
	bool Collision(const Vector2Int position) const;

	Vector2Int GetRandomEmptySpace();
	Vector2Int GetEnemySpace(const Vector2Int fromPosition, const int minDistance);
};
