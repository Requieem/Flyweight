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

class AStar
{
public:
	static bool IsPositionValid(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid, const Vector2Int start, const Vector2Int goal);
	static std::vector<Vector2Int> DirectionPath(const Vector2Int start, const Vector2Int goal, const std::shared_ptr<std::vector<std::vector<bool>>> grid);
};