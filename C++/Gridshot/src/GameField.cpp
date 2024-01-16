#include "GameField.h"

GameField::GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Soldier> player, FieldObjectList* fieldObjects, float fillProbability, int iterations) : size(size), padding(padding), player(player)
{
	this->fieldObjects = fieldObjects;
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
		bool isProjectile = dynamic_cast<Projectile*>(obj.get()) != nullptr;

		if (hit && isProjectile)
		{
				it = fieldObjects->erase(it);
		}
		else ++it;
	}

	return player->IsDead() || IsOutOfBounds(player->Position());
}

#define COLOR_CAVE (COLOR_BLACK | COLOR_RED)

void GameField::Draw() const {
	// Initialize color pairs
	start_color();
	init_color(COLOR_CAVE, 300, 100, 50);
	init_pair(4, COLOR_CAVE, COLOR_BLACK); // Environment
	init_pair(1, COLOR_GREEN, COLOR_BLACK); // Player
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // Enemy
	init_pair(3, COLOR_CYAN, COLOR_CAVE);  // Projectile

	// Clear the screen
	clear();

	attron(COLOR_PAIR(4));

	BoolGrid cave = caveGenerator->GetCave();
	Vector2Int caveSize = caveGenerator->GetSize();

	for (int caveY = 0; caveY < size.y; ++caveY) {
		for (int caveX = 0; caveX < size.x; ++caveX) {
			if (cave[caveY][caveX])
				mvaddch(caveY, caveX, 176);
			else
				mvaddch(caveY, caveX, 219);
		}
	}

	attroff(COLOR_PAIR(4));

	// Drawing logic...
	for (const auto& obj : *fieldObjects) {
		char objectChar = ' ';

		// Set color based on object type
		if (dynamic_cast<Soldier*>(obj.get()) != nullptr) {
			if (obj == player) {
				objectChar = 219;
				attron(COLOR_PAIR(1)); // Set player color
			}
			else {
				objectChar = 219;
				attron(COLOR_PAIR(2)); // Set enemy color
			}
		}
		else if (dynamic_cast<Projectile*>(obj.get()) != nullptr) {
			objectChar = 254;
			attron(COLOR_PAIR(3)); // Set projectile color
		}

		Vector2Int position = obj->Position();

		// Calculate grid position
		int gridX = position.x;
		int gridY = position.y;

		// Print the character
		mvprintw(gridY, gridX, "%c", objectChar);

		// Turn off color
		attroff(COLOR_PAIR(1));
		attroff(COLOR_PAIR(2));
		attroff(COLOR_PAIR(3));
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

	Vector2Int direction = obj->Direction();
	Vector2Int position = obj->Position();

	bool isCrossingBounds = IsCrossingBounds(position, direction);
	bool collidesWithCave = CollidesWithCave(position, direction);
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

bool GameField::IsCrossingBounds(Vector2Int position, Vector2Int direction)
{
	Vector2Int nextPosition = Vector2Int::Sum(position, direction);

	int x = nextPosition.x;
	int y = nextPosition.y;
	int horizontalMax = size.x;
	int verticalMax = size.y;

	return x < 0 || y < 0 || x >= horizontalMax || y >= verticalMax;
}

bool GameField::CollidesWithCave(Vector2Int position, Vector2Int direction)
{
	Vector2Int nextPosition = Vector2Int::Sum(position, direction);
	Vector2Int caveSize = caveGenerator->GetSize();

	int x = nextPosition.x;
	int y = nextPosition.y;
	int xMax = caveSize.x;
	int yMax = caveSize.y;

	bool outOfBounds = x >= xMax || y >= yMax || x < 0 || y < 0;
	BoolGrid cave = caveGenerator->GetCave();
  	return outOfBounds || cave[y][x];
}

