#include "include/Level.h"

void Level::ChangeRoom(Vector2Int dir, std::shared_ptr<Player> player, std::shared_ptr<Vector2Int> position)
{
	Vector2Int pos = currentPosition + dir;

	int x = pos.x;
	int y = pos.y;

	if (!(*roomGrid)[y][x]) 
	{
		auto cave = CurrentRoom()->caveGenerator->GetCave();
		Vector2Int playerPos = player->Position();
		(*cave)[playerPos.y][playerPos.x] = false;
		currentPosition = pos;
		roomMap[currentPosition]->Activate(player, position);
	}
}

Level::Level(Vector2Int size, Vector2Int roomSize) : roomGrid(std::make_shared<std::vector<std::vector<bool>>>())
{
	roomGrid->resize(size.y, std::vector<bool>(size.x));

	while (roomMap.empty())
	{
		for (int y = 0; y < size.y; ++y) {
			for (int x = 0; x < size.x; ++x) {
				bool noRoom = ((rand() / static_cast<float>(RAND_MAX)) < 0.5f);
				(*roomGrid)[y][x] = noRoom;
				if (!noRoom)
				{
					roomMap[{x, y}] = std::make_shared<Room>(roomSize);
				}
			}
		}
	}

	auto emptyGrid = std::make_shared<std::vector<std::vector<bool>>>(size.y, std::vector<bool>(size.x, false));

	for (const auto& begin : roomMap)
	{
		Vector2Int start = begin.first;
		for (const auto& end : roomMap)
		{
			Vector2Int goal = end.first;
			if (start == goal) continue;
			auto path = AStar::DirectionPath(start, goal, roomGrid);
			if (!path.empty()) continue;
			path = AStar::DirectionPath(start, goal, emptyGrid);
			Vector2Int position = start;
			for (Vector2Int dir : path)
			{
				position = position + dir;
				if (roomMap.find(position) == roomMap.end())
				{
					(*roomGrid)[position.y][position.x] = false;
					roomMap[position] = std::make_shared<Room>(roomSize);
				}
			}
		}
	}

	auto it = roomMap.begin();
	std::advance(it, rand() % roomMap.size());
	currentPosition = it->first;
}

std::shared_ptr<Room> Level::CurrentRoom()
{
	int x = currentPosition.x;
	int y = currentPosition.y;

	if (!(*roomGrid)[y][x]) return roomMap[currentPosition];
	else return nullptr;
}

Vector2Int Level::CurrentPosition() { return currentPosition; }
