#include "include/Node.h"

Node::Node(Vector2Int pos, std::shared_ptr<Node> p, int g, int h)
    : position(pos), gCost(g), hCost(h), parent(p) {}

int Node::fCost() const {
    return gCost + hCost;
}

bool CompareNode::operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const
{
    return a->fCost() > b->fCost();
}
