#include <map>

#include "Room.h"

class Level
{
public:
	Vector2Int currentPosition;
	void ChangeRoom(Vector2Int dir, std::shared_ptr<Player> player, std::shared_ptr<Vector2Int> position);
	Level(Vector2Int size, Vector2Int roomSize);
	std::shared_ptr<Room> CurrentRoom();
	Vector2Int CurrentPosition();
	std::shared_ptr<std::vector<std::vector<bool> > > roomGrid;
private:
	std::map<Vector2Int, std::shared_ptr<Room>> roomMap;
};