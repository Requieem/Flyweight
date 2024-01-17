#include "AStar.h"

bool AStar::IsPositionValid(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid, const Vector2Int start, const Vector2Int goal) {
    return position == start || position == goal || (position.y >= 0 && position.y < (*grid).size() &&
        position.x >= 0 && position.x < (*grid)[0].size() &&
        !(*grid)[position.y][position.x]);
}

std::vector<Vector2Int> AStar::DirectionPath(const Vector2Int start, const Vector2Int goal, const std::shared_ptr<std::vector<std::vector<bool>>> grid)
{
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> openSet;
    std::unordered_map<Vector2Int, std::shared_ptr<Node>, Vector2IntHash> allNodes;
    std::unordered_set<Vector2Int, Vector2IntHash> openSetPositions;

    std::shared_ptr<Node> startNode = std::make_shared<Node>(start);
    startNode->gCost = 0;
    startNode->hCost = start.ManhattanDistance(goal);
    openSet.push(startNode);
    allNodes[start] = startNode;
    openSetPositions.insert(start);

    std::vector<Vector2Int> directions = {
        Vector2Int::Right, -Vector2Int::Right, Vector2Int::Up, -Vector2Int::Up
    };

    while (!openSet.empty()) {
        std::shared_ptr<Node> currentNode = openSet.top();
        openSet.pop();
        openSetPositions.erase(currentNode->position);

        if (currentNode->position == goal) {
            std::vector<Vector2Int> path;

            while (currentNode != nullptr) {
                if (currentNode->parent != nullptr) {
                    path.push_back(currentNode->position - currentNode->parent->position);
                }
                currentNode = currentNode->parent;
            }

            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            Vector2Int neighborPos = currentNode->position + dir;

            if (!IsPositionValid(neighborPos, grid, start, goal)) {
                continue;
            }

            int tentativeGCost = currentNode->gCost + 1;
            std::shared_ptr<Node> neighbor = nullptr;

            if (allNodes.find(neighborPos) != allNodes.end()) {
                neighbor = allNodes[neighborPos];
                if (tentativeGCost >= neighbor->gCost) {
                    continue; // Not a better path
                }
            }
            else {
                neighbor = std::make_shared<Node>(neighborPos);
                allNodes[neighborPos] = neighbor;
            }
            neighbor->parent = currentNode;
            neighbor->gCost = tentativeGCost;
            neighbor->hCost = neighborPos.ManhattanDistance(goal);

            if (openSetPositions.find(neighbor->position) == openSetPositions.end()) {
                openSet.push(neighbor);
                openSetPositions.insert(neighbor->position);
            }
        }
    }

    return {};  // Return an empty path if no path is found
}
