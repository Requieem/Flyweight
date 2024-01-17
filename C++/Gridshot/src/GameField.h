#pragma once

#include <stdexcept>

#include "Soldier.h"
#include "FieldObject.h"
#include "CaveGenerator.h"

typedef std::vector<std::shared_ptr<FieldObject>> FieldObjectList;
typedef std::map<Vector2Int, std::vector<std::shared_ptr<FieldObject>>> FieldObjectMap;

class GameField
{
private:
    Vector2Int size;
    Vector2Int padding;
    CaveGenerator* caveGenerator;
    FieldObjectList* fieldObjects;
    std::shared_ptr<Soldier> player;

public:
    bool Tick();
    void Init();
    void Draw() const;
    bool IsOutOfBounds(const Vector2Int position) const;
    bool MakeFieldMove(const std::shared_ptr<FieldObject> object);
    void AddFieldObject(const std::shared_ptr<FieldObject> object);
    GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Soldier> player, FieldObjectList* fieldObjects, float fillProbability, int iterations);
};
