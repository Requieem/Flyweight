#include "include/AStar.h"

//typedef std::shared_ptr<Node> SharedNode;
//typedef std::unordered_set<Vector2Int, Vector2IntHash> PositionSet;
//typedef std::unordered_map<Vector2Int, std::shared_ptr<Node>, Vector2IntHash> NodeMap;
//typedef std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> NodeQueue;

std::unordered_map<
	std::shared_ptr<std::vector<std::vector<bool>>>,
	std::unordered_map<
	Vector2Int,
	std::unordered_map<
	Vector2Int,
	std::vector<Vector2Int>,
	Vector2IntHash
	>,
	Vector2IntHash
	>,
	CaveHash
> AStar::memoizedPaths;

/**
* @brief A predicate that determines whether or not the given position is a valid position of a grid.
*
* Specifically, this function checks if:
* \li The position is within the bounds of the grid AND
* \li The position is not marked as an obstacle in the grid.
*
* @param position The position to check for validity.
* @param grid The grid within which we are executing the check.
*
* @return bool Whether or not the given position is valid and can be traversed.
*
* Example Usage:
* ```
* Vector2Int position(1,3);
* auto grid = std::make_shared<std::vector<std::vector<bool>>>(10, std::vector<bool>(10, true));
* bool isPositionValid = AStar::IsPositionValid(position, grid);
* ```
*/
bool AStar::IsPositionValid(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid) {
	return IsPositionWithinBounds(position, grid) && !IsPositionOccupied(position, grid);
}

/**
* @brief A predicate that determines whether or not the given position is within the bounds of a grid.
*
* Specifically, this function checks if:
* \li The position is within 0 and x or y sizes for the given grid
*
* @param position The position to check for validity.
* @param grid The grid within which we are executing the check.
*
* @return bool Whether or not the given position is within the grid bounds.
*
* Example Usage:
* ```
* Vector2Int position(1,3);
* auto grid = std::make_shared<std::vector<std::vector<bool>>>(10, std::vector<bool>(10, true));
* bool isPositionWithinBounds = AStar::IsPositionWithinBounds(position, grid);
* ```
*/
bool AStar::IsPositionWithinBounds(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid) {
	return (
		position.y >= 0 &&
		position.y < (*grid).size() &&
		position.x >= 0 &&
		position.x < (*grid)[0].size()
		);
}

/**
* @brief A predicate that determines whether or not the given position is occupied with an obstacle.
*
* Specifically, this function checks if:
* \li The value of the grid at the given position equals true
*
* @param position The position to check for validity.
* @param grid The grid within which we are executing the check.
*
* @return bool Whether or not the given position in the grid is occupied.
*
* Example Usage:
* ```
* Vector2Int position(1,3);
* auto grid = std::make_shared<std::vector<std::vector<bool>>>(10, std::vector<bool>(10, true));
* bool isPositionOccupied = AStar::IsPositionOccupied(position, grid);
* ```
*/
bool AStar::IsPositionOccupied(const Vector2Int position, const std::shared_ptr<std::vector<std::vector<bool>>> grid) {
	return (*grid)[position.y][position.x];
}

/**
 * @brief Calculates a path made up of directions on a 2D grid using the A* algorithm.
 *
 * This function implements a variant of the A* pathfinding algorithm to find a
 * path from a start point to a goal on a 2D grid. It returns a list of directions
 * to take to reach the goal. If no path is found, an empty list is returned.
 *
 * @param start The starting point of the path on the grid.
 * @param goal The goal point of the path on the grid.
 * @param grid A shared pointer to a 2D grid represented as a vector of vector of bools,
 *             where `true` indicates a walkable cell and `false` indicates an obstacle.
 * @return std::vector<Vector2Int> A vector of directions (as Vector2Int) forming the path from start to goal.
 * @see Vector2Int for additional details about this 2D Vector implementation.
 *
 * @note The function assumes the grid is a non-null, valid 2D grid.
 * @warning If start or goal are outside of grid bounds, an empty path is returned.
 *
 * Example usage:
 * ```
 * Vector2Int start(0, 0);
 * Vector2Int goal(5, 5);
 * auto grid = std::make_shared<std::vector<std::vector<bool>>>(10, std::vector<bool>(10, true));
 * auto path = AStar::DirectionPath(start, goal, grid);
 * ```
 */
std::vector<Vector2Int> AStar::DirectionPath(const Vector2Int start, const Vector2Int goal, const std::shared_ptr<std::vector<std::vector<bool>>> grid)
{
	// If start or goal are not within grid bounds, return an empty path
	if (!IsPositionWithinBounds(start, grid) || !IsPositionWithinBounds(goal, grid))
	{
		return {};
	}

	// If a path from start to goal in this grid was already computed, return it from the memoization map.
	if (AStar::memoizedPaths.find(grid) != AStar::memoizedPaths.end() &&
		AStar::memoizedPaths[grid].find(start) != AStar::memoizedPaths[grid].end() &&
		AStar::memoizedPaths[grid][start].find(goal) != AStar::memoizedPaths[grid][start].end())
	{
		return AStar::memoizedPaths[grid][start][goal];
	}

	// Declaring local map, queue and set used for A* logic
	std::unordered_map<Vector2Int, std::shared_ptr<Node>, Vector2IntHash> allNodes;
	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNode> openQueue;
	std::unordered_set<Vector2Int, Vector2IntHash> positions;

	// Declaring starting node
	std::shared_ptr<Node> startNode = std::make_shared<Node>(start);
	startNode->gCost = 0;
	startNode->hCost = start.ManhattanDistance(goal);

	// Push and store the starting node and its position
	openQueue.push(startNode);
	allNodes[start] = startNode;
	positions.insert(start);

	// Continue until there are queued nodes
	while (!openQueue.empty()) {
		// The current node is on top of the queue
		std::shared_ptr<Node> currentNode = openQueue.top();

		// Remove the current node from both node queue and position set
		openQueue.pop();
		positions.erase(currentNode->position);

		// We reached the goal
		if (currentNode->position == goal) {
			std::vector<Vector2Int> path;

			// Travel up the path made of this node's ancestors
			while (currentNode != nullptr) {
				if (currentNode->parent != nullptr) {
					// Push the direction from the current node to its parent in the path
					path.push_back(currentNode->position - currentNode->parent->position);
				}
				currentNode = currentNode->parent;
			}

			std::reverse(path.begin(), path.end());		// Reverse the path cause we need it from start to goal
			AStar::AStar::memoizedPaths[grid][start][goal] = path;	// Memoize the computed path for later use

			return path;
		}

		// We haven't reached the goal
		// Iterate over directions to queue all neighbors
		for (const auto& dir : Vector2Int::Directions) {
			Vector2Int neighborPos = currentNode->position + dir;

			// Note: goal and start might be invalid positions
			//		 in the grid, because they are likely occupied
			//		 by the requesting or target entity
			//		 thus we assume those positions valid
			if ((neighborPos != goal && neighborPos != start) &&
				!IsPositionValid(neighborPos, grid))
			{
				continue;
			}

			// We assume minimal gCost
			int tentativeGCost = currentNode->gCost + 1;
			std::shared_ptr<Node> neighbor = nullptr;

			// This node has already been visited
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

			// Note: remember this only executes if it's a better path
			// Connect this neighbor to the current node
			neighbor->parent = currentNode;
			neighbor->gCost = tentativeGCost;
			neighbor->hCost = neighborPos.ManhattanDistance(goal);

			// Push the neighbor if not yet present
			if (positions.find(neighbor->position) == positions.end()) {
				openQueue.push(neighbor);
				positions.insert(neighbor->position);
			}
		}
	}

	return {};  // Return an empty path if no path is found
}
