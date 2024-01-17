#include <memory>
#include "Vector2Int.h"

struct Node {
    int gCost;
    int hCost;
    std::shared_ptr<Node> parent;
    Vector2Int position;

    Node(Vector2Int pos, std::shared_ptr<Node> p = nullptr, int g = 0, int h = 0);
    int fCost() const;
};

struct CompareNode {
    bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const;
};