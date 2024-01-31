#include "../include/CaveGenerator.h"

CaveGenerator::CaveGenerator(Vector2Int size, float fillProbability) : size(size), fillProbability(fillProbability)
{
	GenerateInitialCave();
}

Vector2Int CaveGenerator::GetSize() const { return size; }
int CaveGenerator::GetEmptySpaces() const { return emptySpaces.size(); }
std::shared_ptr<std::vector<std::vector<bool>>> CaveGenerator::GetCave() const { return cave; }

void CaveGenerator::GenerateInitialCave()
{
	// Use high-resolution clock to seed the random number generator
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
	srand(static_cast<unsigned int>(nanoseconds));
	
	cave = std::make_shared<std::vector<std::vector<bool>>>();
	cave->resize(size.y, std::vector<bool>(size.x));
	for (int y = 0; y < size.y; ++y) {
		for (int x = 0; x < size.x; ++x) {
			(*cave)[y][x] = (rand() / static_cast<float>(RAND_MAX)) < fillProbability;
		}
	}
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
			else if ((*cave)[ny][nx]) {
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
				(*newCave)[y][x] = true;
			}
			else if (solidNeighbors < 4) {
				(*newCave)[y][x] = false;
				emptySpaces.push_back({ x, y });
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
	(*cave)[randomPosition.y][randomPosition.x] = true;
	return randomPosition;
}

Vector2Int CaveGenerator::GetEnemySpace(const Vector2Int fromPosition, const int minDistance) {

	Vector2Int randomPosition = GetRandomDistantEmptySpace(fromPosition, minDistance);
	(*cave)[randomPosition.y][randomPosition.x] = true;
	return randomPosition;
}

Vector2Int CaveGenerator::GetRandomDistantEmptySpace(const Vector2Int fromPosition, const int minDistance) {

	if (emptySpaces.size() == 0) return Vector2Int::Zero;

	int distance;
	int randomIndex;
	Vector2Int randomPosition;

	do {

		randomIndex = rand() % emptySpaces.size();
		randomPosition = emptySpaces[randomIndex];
		distance = Vector2Int::Distance(fromPosition, randomPosition);
		if (distance >= minDistance) emptySpaces.erase(emptySpaces.begin() + randomIndex);

	} while (distance < minDistance && !emptySpaces.empty());

	emptySpaces.erase(emptySpaces.begin() + randomIndex);
	return randomPosition;
}

std::size_t CaveHash::operator()(const std::shared_ptr<std::vector<std::vector<bool>>>& grid) const noexcept
{
	// Hash the pointer's address
	return std::hash<std::vector<std::vector<bool>>*>{}(grid.get());
}
