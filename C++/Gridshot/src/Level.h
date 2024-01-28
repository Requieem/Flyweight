#include <map>

#include "Room.h"

class Level
{
public:
	Vector2Int currentPosition;
	std::shared_ptr<Room> AdjacentRoom(Vector2Int dir);
	Level(Vector2Int size, Vector2Int roomSize);
	std::shared_ptr<Room> CurrentRoom();
	std::shared_ptr<std::vector<std::vector<bool>>> roomGrid;
private:
	std::map<Vector2Int, std::shared_ptr<Room>> roomMap;
};