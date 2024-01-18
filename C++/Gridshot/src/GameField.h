#pragma once

#include <stdexcept>

#include "Soldier.h"
#include "FieldObject.h"
#include "CaveGenerator.h"

typedef std::shared_ptr<std::vector<std::shared_ptr<FieldObject>>> FieldObjectList;
typedef std::shared_ptr < std::map<Vector2Int, std::shared_ptr<FieldObject>>> ObjectMap;

class GameField
{
private:
    Vector2Int size;
    Vector2Int padding;
    CaveGenerator* caveGenerator;
    FieldObjectList fieldObjects;
    ObjectMap objectMap;
    std::shared_ptr<Soldier> player;

public:
    bool Tick();
    void Init();
    void Draw() const;
    bool IsOutOfBounds(const Vector2Int position) const;
    bool MakeFieldMove(const std::shared_ptr<FieldObject> object);
    void AddFieldObject(const std::shared_ptr<FieldObject> object);
    std::shared_ptr<FieldObject> Hit(std::shared_ptr<FieldObject> obj, Vector2Int pos) const;
    bool CheckHit(const std::shared_ptr<FieldObject> obj) const;
    bool ReachedPlayer(const std::shared_ptr<FieldObject> obj) const;
    GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Soldier> player, FieldObjectList fieldObjects, float fillProbability, int iterations);
};
