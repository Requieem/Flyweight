#include "include/Room.h"

const int Room::minHearts = 2;
const int Room::maxHearts = 5;
const int Room::iterations = 25;
const int Room::minEnemies = 10;
const int Room::maxEnemies = 25;
const int Room::minTreasures = 5;
const int Room::maxTreasures = 15;
const float Room::fillProbability = 0.45f;

Room::Room(Vector2Int size)
{
	activated = false;
	caveGenerator = new CaveGenerator(size, fillProbability);
	caveGenerator->GenerateCave(iterations, nullptr, nullptr);
	itemMap = std::make_shared<std::map<Vector2Int, std::shared_ptr<Item>>>();
	fieldObjects = std::make_shared<std::vector<std::shared_ptr<FieldObject>>>();
	objectMap = std::make_shared <std::map<Vector2Int, std::shared_ptr<FieldObject>>>();
}

void Room::AddFieldObject(const std::shared_ptr<FieldObject> object)
{
	object->SetCave(caveGenerator->GetCave());
	(*objectMap)[object->Position()] = object;
	fieldObjects->push_back(object);
}

void Room::Activate(std::shared_ptr<Player> player, std::shared_ptr<Vector2Int> position)
{	
	Vector2Int playerStartingPos;
	auto cave = caveGenerator->GetCave();
	

	if (position == nullptr)
	{
		playerStartingPos = caveGenerator->GetRandomEmptySpace();
	}
	else
	{
		playerStartingPos = *position;
		Vector2Int caveSize = caveGenerator->GetSize();
		int x = playerStartingPos.x;
		int y = playerStartingPos.y;
		(*cave)[y][x] = true;

		// Carve a hole around the player
		for (int i = std::max(0, y - 2); i < std::min(caveSize.y, y + 2); i++) {
			for (int j = std::max(0, x - 2); j < std::min(caveSize.x, x + 2); j++) {
				int dy = std::abs(y - i);
				int dx = std::abs(x - j);
				if (dx + dy > 2) continue; // Make the hole 'circular'
				if (i == y && j == x) continue; // Ignore the player position
				(*cave)[i][j] = false;
			}
		}
	}

		player->SetCave(cave);
		player->SetPosition(playerStartingPos);
		(*objectMap)[playerStartingPos] = player;
		if(activated) return;
		fieldObjects->push_back(player);


		std::random_device rd;
		std::mt19937 heartGen(rd());
		std::mt19937 enemyGen(rd());
		std::mt19937 treasureGen(rd());

		std::uniform_int_distribution<int> enemyDistribution(Room::minEnemies, Room::maxEnemies);
		int numEnemies = enemyDistribution(enemyGen);

		std::uniform_int_distribution<int> treasureDistribution(Room::minTreasures, Room::maxTreasures);
		int numTreasures = treasureDistribution(treasureGen);

		std::uniform_int_distribution<int> heartDistribution(Room::minHearts, Room::maxHearts);
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
			enemy->SetCave(cave);
			(*objectMap)[enemy->Position()] = enemy;
	}

	activated = true;
}
