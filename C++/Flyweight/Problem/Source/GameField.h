#pragma once
#include <iostream>
#include "Soldier.h"
#include "FieldObject.h"

class GameField
{
private:
    std::shared_ptr<Soldier> player;
    Vector2Int size;
    FieldObjectMap objectMap;
    FieldObjectList* fieldObjects;

public:
    GameField(Vector2Int size, std::shared_ptr<Soldier> player, FieldObjectList* fieldObjects);
    bool Tick();
    void Draw() const;
    bool IsOutOfBounds(Vector2Int position) const;
    bool MakeFieldMove(const SharedFieldObject& object);
    bool IsCrossingBounds(Vector2Int position, Vector2Int move);
};
