#pragma once

#include <stdexcept>

#include "Soldier.h"
#include "FieldObject.h"
#include "CaveGenerator.h"

typedef std::vector<std::shared_ptr<FieldObject>> FieldObjectList;
typedef std::map<Vector2Int, std::vector<std::shared_ptr<FieldObject>>> FieldObjectMap;
using SharedFieldObject = std::shared_ptr<FieldObject>;
using SharedSoldier = std::shared_ptr<Soldier>;

class GameField
{
private:
    Vector2Int size;
    Vector2Int padding;
    FieldObjectMap objectMap;
    CaveGenerator* caveGenerator;
    FieldObjectList* fieldObjects;
    SharedSoldier player;

public:
    bool Tick();
    void Draw() const;
    bool IsOutOfBounds(const Vector2Int position) const;
    bool MakeFieldMove(const SharedFieldObject& object);
    GameField(Vector2Int size, Vector2Int padding, SharedSoldier player, FieldObjectList* fieldObjects, float fillProbability, int iterations);
};
