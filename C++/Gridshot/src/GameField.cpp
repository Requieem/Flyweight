#include "GameField.h"

GameField::GameField(Vector2Int size, Vector2Int padding, SharedSoldier player, FieldObjectList* fieldObjects, float fillProbability, int iterations) : size(size), padding(padding), player(player), fieldObjects(fieldObjects)
{
	this->caveGenerator = new CaveGenerator(size, fillProbability);
	caveGenerator->GenerateCave(iterations, nullptr, nullptr);

	Vector2Int playerStartingPos = caveGenerator->GetRandomEmptySpace();

	for (const SharedFieldObject& current : *fieldObjects)
	{
		if (current == player) continue;

		Vector2Int randomEmptySpace = caveGenerator->GetEnemySpace(playerStartingPos, 3);
		current->SetPosition(randomEmptySpace);

		objectMap[current->Position()].push_back(current);
	}

	player->SetPosition(playerStartingPos);
	objectMap[player->Position()].push_back(player);
}

bool GameField::Tick()
{
	for (auto it = fieldObjects->begin(); it != fieldObjects->end();) {
		const SharedFieldObject& obj = *it;

		bool hit = MakeFieldMove(obj);
		bool remove = obj->RemoveOnCollision();

		if (hit && remove)
		{
				it = fieldObjects->erase(it);
		}

		else ++it;
	}

	return player->IsDead() || IsOutOfBounds(player->Position());
}

void GameField::Draw() const {
	// Initialize color pairs
	start_color();
	init_color(CAVE_COLOR, 300, 100, 50);
	init_pair(1, CAVE_COLOR, COLOR_BLACK); // Environment

	// Clear the screen
	clear();

	attron(COLOR_PAIR(1));

	Cave cave = caveGenerator->GetCave();
	Vector2Int caveSize = caveGenerator->GetSize();

	for (int caveY = 0; caveY < size.y; ++caveY) {
		for (int caveX = 0; caveX < size.x; ++caveX) {
			if (cave[caveY][caveX])
				mvaddch(caveY, caveX, 176);
			else
				mvaddch(caveY, caveX, 219);
		}
	}

	attroff(COLOR_PAIR(1));

	std::map<std::pair<int, int>, int> colorMap;
	int nextPair = 2;

	// Drawing logic...
	for (const auto& obj : *fieldObjects) {
		int color = obj->TeamColor();
		int background = obj->BackgroundColor();
		char objectChar = obj->DisplayChar();

		std::pair<int, int> colorCombination = { color, background };

		if (colorMap.find(colorCombination) == colorMap.end()) {
			init_pair(nextPair, color, background);
			colorMap[colorCombination] = nextPair;
			nextPair++;
		}

		int n = colorMap[colorCombination];
		attron(COLOR_PAIR(n));

		Vector2Int position = obj->Position();

		// Calculate grid position
		int gridX = position.x;
		int gridY = position.y;

		// Print the character
		mvprintw(gridY, gridX, "%c", objectChar);

		// Turn off color
		attroff(COLOR_PAIR(n));
	}

	// Refresh to update the screen
	refresh();
}

bool GameField::IsOutOfBounds(Vector2Int position) const
{
	int x = position.x;
	int y = position.y;
	int horizontalMax = size.x;
	int verticalMax = size.y;

	return x < 0 || y < 0 || x >= horizontalMax || y >= verticalMax;
}
bool GameField::MakeFieldMove(const SharedFieldObject& obj)
{
	if (!obj->IsMoving()) return false;

	Vector2Int position = obj->Position();
	Vector2Int nextPosition = obj->NextPosition();

	bool isCrossingBounds = IsOutOfBounds(nextPosition);
	bool collidesWithCave = caveGenerator->Collision(nextPosition);

	auto pairsIterator = objectMap.find(position);

	if (pairsIterator != objectMap.end())
	{
		FieldObjectList objectsAtThisPosition = pairsIterator->second;
		auto valueIterator = std::find(objectsAtThisPosition.begin(), objectsAtThisPosition.end(), obj);

		if (valueIterator != objectsAtThisPosition.end())
		{
			objectsAtThisPosition.erase(valueIterator);
		}
	}

	if (isCrossingBounds || collidesWithCave) return true;

	obj->Move();
	position = obj->Position();
	objectMap[position].push_back(obj);

	return false;
}