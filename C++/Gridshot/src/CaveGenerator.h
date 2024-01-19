#pragma once

#include <ctime>
#include <vector>
#include <memory>
#include <cstdlib>

#include "Vector2Int.h"

class CaveGenerator
{
private:
	std::shared_ptr<std::vector<std::vector<bool>>> cave;

	float fillProbability;
	Vector2Int size;
	std::vector<Vector2Int> emptySpaces;

	void CaveIteration();
	void GenerateInitialCave();
	int CountSolidNeighbors(const int x, const int y) const;

public:
	CaveGenerator(Vector2Int size, float fillProbability);

	void GenerateCave(const int iterations, const Vector2Int* size, const float* fillProbability);

	std::shared_ptr<std::vector<std::vector<bool>>> GetCave() const;
	Vector2Int GetSize() const;
	int GetEmptySpaces() const;
	Vector2Int GetRandomEmptySpace();
	Vector2Int GetEnemySpace(const Vector2Int fromPosition, const int minDistance);
	Vector2Int GetRandomDistantEmptySpace(const Vector2Int fromPosition, const int minDistance);
};
