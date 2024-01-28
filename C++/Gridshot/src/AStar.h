#include <queue>
#include <cmath>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "Node.h"
#include "Vector2Int.h"
#include "CaveGenerator.h"

class AStar
{
private:
    static std::unordered_map<std::shared_ptr<std::vector<std::vector<bool>>>,std::unordered_map<Vector2Int,std::unordered_map<Vector2Int,std::vector<Vector2Int>,Vector2IntHash>,Vector2IntHash>,CaveHash> memoizedPaths;
	static bool IsPositionValid(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid);
	static bool IsPositionOccupied(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid);
	static bool IsPositionWithinBounds(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid);
public:
	static std::vector<Vector2Int> DirectionPath(const Vector2Int start, const Vector2Int goal, const std::shared_ptr<std::vector<std::vector<bool>>> grid);
};