#include "CaveGenerator.h"

CaveGenerator::CaveGenerator(Vector2Int size, float fillProbability) : size(size), fillProbability(fillProbability)
{
	srand(time(nullptr));
	GenerateInitialCave();
}

Cave CaveGenerator::GetCave() const { return cave; }
Cave CaveGenerator::GetMinusCave() const { return minusCave; }
Vector2Int CaveGenerator::GetSize() const { return size; }

void CaveGenerator::GenerateInitialCave()
{
	cave.resize(size.y, std::vector<bool>(size.x));
	minusCave.resize(size.y, std::vector<bool>(size.x));
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			cave[y][x] = (rand() / static_cast<float>(RAND_MAX)) < fillProbability;
			minusCave[y][x] = (rand() / static_cast<float>(RAND_MAX)) < fillProbability;
		}
	}
}

bool CaveGenerator::Collision(const Vector2Int position) const
{
	int x = position.x;
	int y = position.y;
	int maxX = size.x;
	int maxY = size.y;

	bool outOfBounds = x < 0 || y < 0 || x >= maxX || y >= maxY;
	return outOfBounds || cave[y][x];
}

int CaveGenerator::CountSolidNeighbors(const int x, const int y) const
{
	int count = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			int nx = x + i;
			int ny = y + j;
			if (i == 0 && j == 0) continue; // Skip the center cell
			if (nx < 0 || nx >= size.x || ny < 0 || ny >= size.y) {
				count++; // Treat out of bounds as solid
			}
			else if (cave[ny][nx]) {
				count++;
			}
		}
	}
	return count;
}

void CaveGenerator::CaveIteration()
{
	auto newCave = cave;
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			int solidNeighbors = CountSolidNeighbors(x, y);
			if (solidNeighbors > 4) {
				newCave[y][x] = true;
				minusCave[y][x] = false;
			}
			else if (solidNeighbors < 4) {
				newCave[y][x] = false;
				minusCave[y][x] = true;
				emptySpaces.push_back(*new Vector2Int(x, y));
			}
		}
	}
	cave = newCave;
}

void CaveGenerator::GenerateCave(const int iterations, const Vector2Int* size, const float* fillProbability)
{
	if (size != nullptr)
		this->size = *size;
	if (fillProbability != nullptr)
		this->fillProbability = *fillProbability;

	for (int i = 0; i < iterations; ++i) {
		CaveIteration();
	}
}

Vector2Int CaveGenerator::GetRandomEmptySpace()
{
	int randomIndex = rand() % emptySpaces.size();
	Vector2Int randomPosition = emptySpaces[randomIndex];
	emptySpaces.erase(emptySpaces.begin() + randomIndex);
	minusCave[randomPosition.y][randomPosition.x] = false;
	return randomPosition;
}

Vector2Int CaveGenerator::GetEnemySpace(const Vector2Int fromPosition, const int minDistance) {
	int randomIndex;
	int distance;
	Vector2Int randomPosition;

	do {
		randomIndex = rand() % emptySpaces.size();
		randomPosition = emptySpaces[randomIndex];
		distance = Vector2Int::Distance(fromPosition, randomPosition);
	} while (distance < minDistance);

	emptySpaces.erase(emptySpaces.begin() + randomIndex);
	minusCave[randomPosition.y][randomPosition.x] = false;
	return randomPosition;
}
