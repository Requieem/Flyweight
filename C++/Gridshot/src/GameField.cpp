#include "GameField.h"

GameField::GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Soldier> player, FieldObjectList fieldObjects, float fillProbability, int iterations) : size(size), padding(padding), player(player), fieldObjects(fieldObjects)
{
	this->caveGenerator = new CaveGenerator(size, fillProbability);
	this->objectMap = std::make_shared <std::map<Vector2Int, std::shared_ptr<FieldObject>>>();

	caveGenerator->GenerateCave(iterations, nullptr, nullptr);
	Vector2Int playerStartingPos = caveGenerator->GetRandomEmptySpace();

	for (const std::shared_ptr<FieldObject>& current : *fieldObjects)
	{
		if (current == player) continue;

		Vector2Int randomEmptySpace = caveGenerator->GetEnemySpace(playerStartingPos, 3);
		current->SetPosition(randomEmptySpace);
		(*objectMap)[current->Position()] = current;
	}

	player->SetPosition(playerStartingPos);
	(*objectMap)[playerStartingPos] = player;
}

void GameField::Init()
{
	for (const std::shared_ptr<FieldObject> current : *fieldObjects)
	{
		current->SetCave(caveGenerator->GetCave());
	}
}

bool GameField::Tick()
{
	for (auto it = fieldObjects->begin(); it != fieldObjects->end();) {
		const std::shared_ptr<FieldObject>& obj = *it;

		bool reachedPlayer = obj->TeamColor() == ENEMY_COLOR && ReachedPlayer(obj);
		bool deadAfterHit = reachedPlayer || CheckHit(obj);
		bool caveHit = reachedPlayer || MakeFieldMove(obj);
		bool remove = obj->RemoveOnCollision();

		if (reachedPlayer)
		{
			player->ChangeHealth(-obj->GetDamage());
		}

		if ((caveHit && remove) || deadAfterHit || reachedPlayer)
		{
			Vector2Int position = obj->Position();
			it = fieldObjects->erase(it);
			(*objectMap).erase(position);
			(*caveGenerator->GetCave())[position.y][position.x] = false;
		}

		else ++it;
	}

	return player->IsDead() || IsOutOfBounds(player->Position());
}

void GameField::Draw() const {
	start_color();
	init_color(CAVE_COLOR, 300, 100, 50);
	init_pair(1, CAVE_COLOR, COLOR_BLACK); // Environment

	// Clear the screen
	clear();
	curs_set(0);

	attron(COLOR_PAIR(1));

	std::shared_ptr<std::vector<std::vector<bool>>> cave = caveGenerator->GetCave();
	std::shared_ptr<std::vector<std::vector<bool>>> field = caveGenerator->GetCave();
	Vector2Int caveSize = caveGenerator->GetSize();

	for (int caveY = 0; caveY < size.y; ++caveY) {
		for (int caveX = 0; caveX < size.x; ++caveX) {
			if ((*cave)[caveY][caveX])
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

bool GameField::MakeFieldMove(const std::shared_ptr<FieldObject> obj)
{
	if (!obj->IsMoving()) return false;

	Vector2Int direction = obj->Direction();
	bool collidesWithCave = obj->WillCollideWithCave(direction);
	
	if (collidesWithCave) return true;

	objectMap->erase(obj->Position());
	obj->Move(direction);
	(*objectMap)[obj->Position()] = obj;
	return false;
}

bool GameField::CheckHit(const std::shared_ptr<FieldObject> obj) const
{
	if (!obj->IsMoving()) return false;
	Vector2Int direction = obj->NextDirection();

	std::shared_ptr<FieldObject> hit = Hit(obj, obj->NextPosition(direction));
	std::map<std::shared_ptr<FieldObject>, bool> hitMap;
	hitMap[obj] = true;

	while (hit != nullptr && hitMap.find(hit) == hitMap.end())
	{
		hitMap[hit] = true;
		hit->ChangeHealth(-obj->GetDamage());
		hit = Hit(hit, hit->NextPosition(hit->Direction()));
	}

	return obj->IsDead();
}

bool GameField::ReachedPlayer(const std::shared_ptr<FieldObject> obj) const
{
	if (obj == player) return false;
	Vector2Int pos = obj->Position();
	Vector2Int playerPos = player->Position();

	return (pos + Vector2Int::Up == playerPos
		|| pos + Vector2Int::Down == playerPos
		|| pos + Vector2Int::Right == playerPos
		|| pos + Vector2Int::Left == playerPos);
}

void GameField::AddFieldObject(const std::shared_ptr<FieldObject> object)
{
	object->SetCave(caveGenerator->GetCave());
	(*objectMap)[object->Position()] = object;
	fieldObjects->push_back(object);
}

std::shared_ptr<FieldObject> GameField::Hit(std::shared_ptr<FieldObject> obj, Vector2Int pos) const
{
	if (objectMap->find(pos) != objectMap->end() && (*objectMap)[pos] != obj)
	{
		return (*objectMap)[pos];
	}

	return nullptr;
}
