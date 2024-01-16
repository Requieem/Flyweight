#pragma once

#include PLATFORM_HEADER
#include <iostream>
#include "Soldier.h"
#include "Projectile.h"
#include "FieldObject.h"
#include "CaveGenerator.h"
#include <stdexcept>

class GameField
{
private:
    Vector2Int size;
    Vector2Int padding;
    FieldObjectMap objectMap;
    CaveGenerator* caveGenerator;
    FieldObjectList* fieldObjects;
    std::shared_ptr<Soldier> player;

public:
    bool Tick();
    void Draw() const;
    bool IsOutOfBounds(Vector2Int position) const;
    bool MakeFieldMove(const SharedFieldObject& object);
    bool IsCrossingBounds(Vector2Int position, Vector2Int move);
    bool CollidesWithCave(Vector2Int position, Vector2Int move);
    GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Soldier> player, FieldObjectList* fieldObjects, float fillProbability, int iterations);
};
