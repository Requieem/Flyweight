#include "GameField.h"

GameField::GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Player> player, float fillProbability, int iterations) : size(size), padding(padding), player(player)
{
	this->caveGenerator = new CaveGenerator(size, fillProbability);
	this->itemMap = std::make_shared<std::map<Vector2Int, std::shared_ptr<Item>>>();
	this->fieldObjects = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();
	this->objectMap = std::make_shared <std::map<Vector2Int, std::shared_ptr<FieldObject>>>();

	caveGenerator->GenerateCave(iterations, nullptr, nullptr);

	Vector2Int playerStartingPos = caveGenerator->GetRandomEmptySpace();
	player->SetPosition(playerStartingPos);
	(*objectMap)[playerStartingPos] = player;
	fieldObjects->push_back(player);

	const int minEnemies = 10;
	const int maxEnemies = 25;
	const int minTreasures = 5;
	const int maxTreasures = 15;
	const int minHearts = 2;
	const int maxHearts = 5;

	std::random_device rd;
	std::mt19937 heartGen(rd());
	std::mt19937 enemyGen(rd());
	std::mt19937 treasureGen(rd());

	std::uniform_int_distribution<int> enemyDistribution(minEnemies, maxEnemies);
	int numEnemies = enemyDistribution(enemyGen);

	std::uniform_int_distribution<int> treasureDistribution(minTreasures, maxTreasures);
	int numTreasures = treasureDistribution(treasureGen);

	std::uniform_int_distribution<int> heartDistribution(minHearts, maxHearts);
	int numHearts = heartDistribution(heartGen);

	for (int i = 0; i < numTreasures; ++i) {
		if (caveGenerator->GetEmptySpaces() == 0) break;
		Vector2Int randomEmptySpace = caveGenerator->GetRandomDistantEmptySpace(playerStartingPos, 3);
		if (randomEmptySpace == Vector2Int::Zero) break;

		std::shared_ptr<Treasure> treasure = std::make_shared<Treasure>(1);
		(*itemMap)[randomEmptySpace] = treasure;
	}

	for (int i = 0; i < numHearts; ++i) {
		if (caveGenerator->GetEmptySpaces() == 0) break;
		Vector2Int randomEmptySpace = caveGenerator->GetRandomDistantEmptySpace(playerStartingPos, 3);
		if (randomEmptySpace == Vector2Int::Zero) break;

		std::shared_ptr<Food> treasure = std::make_shared<Food>(1);
		(*itemMap)[randomEmptySpace] = treasure;
	}

	for (int i = 0; i < numEnemies; ++i) {
		if (caveGenerator->GetEmptySpaces() == 0) break;
		Vector2Int randomEmptySpace = caveGenerator->GetEnemySpace(playerStartingPos, 10);
		if (randomEmptySpace == Vector2Int::Zero) break;

		std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(ENEMY_COLOR, 1, 10, Vector2Int(0, 0), Vector2Int(0, 0), player);
		fieldObjects->push_back(enemy);
		enemy->SetPosition(randomEmptySpace);
		(*objectMap)[enemy->Position()] = enemy;
	}

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
	clear();
	curs_set(0);
	attron(COLOR_PAIR(1));

	std::shared_ptr<std::vector<std::vector<bool>>> cave = caveGenerator->GetCave();
	std::shared_ptr<std::vector<std::vector<bool>>> field = caveGenerator->GetCave();
	Vector2Int caveSize = caveGenerator->GetSize();

	for (int caveY = 0; caveY < size.y; ++caveY) {
		for (int caveX = 0; caveX < size.x; ++caveX) {
			if (objectMap->find({ caveX, caveY }) != objectMap->end()) continue;

			if ((*cave)[caveY][caveX])
			{
				mvaddch(caveY, caveX, 176);
			}
			else
			{
				auto mapEnd = objectMap->end();
				if ((caveY <= size.y - 2 && (*cave)[caveY + 1][caveX] && objectMap->find({caveX, caveY + 1}) == mapEnd) ||
					(caveX <= size.x - 2 && (*cave)[caveY][caveX + 1] && objectMap->find({ caveX + 1, caveY }) == mapEnd) ||
					((caveX <= size.x - 2 && caveY <= size.y - 2) && (*cave)[caveY + 1][caveX + 1] && objectMap->find({ caveX + 1, caveY + 1 }) == mapEnd) ||
					(caveY >= 1 && (*cave)[caveY - 1][caveX] && objectMap->find({ caveX, caveY - 1 }) == mapEnd) ||
					(caveX >= 1 && (*cave)[caveY][caveX - 1] && objectMap->find({ caveX - 1, caveY }) == mapEnd) ||
					((caveX >= 1 && caveY >= 1) && (*cave)[caveY - 1][caveX - 1] && objectMap->find({ caveX - 1, caveY - 1 }) == mapEnd) ||
					((caveX >= 1 && caveY <= size.y - 2) && (*cave)[caveY + 1][caveX - 1] && objectMap->find({ caveX - 1, caveY + 1 }) == mapEnd) ||
					((caveY >= 1 && caveX <= size.x - 2) && (*cave)[caveY - 1][caveX + 1] && objectMap->find({ caveX + 1, caveY - 1 }) == mapEnd))
				{
					mvaddch(caveY, caveX, 178);
				}
				else
				{
					mvaddch(caveY, caveX, 219);
				}

			}
		}
	}

	attroff(COLOR_PAIR(1));

	std::map<std::pair<int, int>, int> colorMap;
	int nextPair = 5;

	// Drawing logic...
	for (const auto& obj : *fieldObjects) {
		int color = obj->TeamColor();
		unsigned char objectChar = obj->DisplayChar();

		std::pair<int, int> colorCombination = { color, CAVE_COLOR };

		if (colorMap.find(colorCombination) == colorMap.end()) {
			init_pair(nextPair, color, CAVE_COLOR);
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

	for (const auto& [pos, item] : *itemMap) {
		int color = item->Color();
		unsigned char objectChar = item->DisplayChar();

		std::pair<int, int> colorCombination = { color, CAVE_COLOR };

		if (colorMap.find(colorCombination) == colorMap.end()) {
			init_pair(nextPair, color, CAVE_COLOR);
			colorMap[colorCombination] = nextPair;
			nextPair++;
		}

		int n = colorMap[colorCombination];
		attron(COLOR_PAIR(n));

		// Calculate grid position
		int gridX = pos.x;
		int gridY = pos.y;

		// Print the character
		mvprintw(gridY, gridX, "%c", objectChar);

		// Turn off color
		attroff(COLOR_PAIR(n));
	}

	attron(COLOR_PAIR(0));

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
	Vector2Int position = obj->Position();
	if (obj == player && itemMap->find(position) != itemMap->end())
	{
		(*itemMap)[position]->Effect(player);
		itemMap->erase(position);
	}
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
