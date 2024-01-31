#include <map>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>

#include "Item.h"
#include "Food.h"
#include "Enemy.h"
#include "Treasure.h"
#include "Vector2Int.h"
#include "FieldObject.h"
#include "CaveGenerator.h"


typedef std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> FieldObjectList;
typedef std::shared_ptr<std::map<Vector2Int, std::shared_ptr<FieldObject>>> ObjectMap;
typedef std::shared_ptr<std::map<Vector2Int, std::shared_ptr<Item>>> ItemMap;

class Room
{
public:
	Room(Vector2Int size);
	void Activate(std::shared_ptr<Player> player, std::shared_ptr<Vector2Int> position);
	void AddFieldObject(const std::shared_ptr<FieldObject> object);
    ItemMap itemMap;
    ObjectMap objectMap;
    CaveGenerator* caveGenerator;
    FieldObjectList fieldObjects;
private:
	bool activated;
	static const int minHearts;
	static const int maxHearts;
	static const int iterations;
	static const int minEnemies;
	static const int maxEnemies;
	static const int minTreasures;
	static const int maxTreasures;
	static const float fillProbability;
};