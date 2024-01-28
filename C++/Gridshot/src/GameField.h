#pragma once

#include <random>
#include <stdexcept>

#include "Item.h"
#include "Food.h"
#include "Enemy.h"
#include "Player.h"
#include "Level.h"
#include "Treasure.h"
#include "FieldObject.h"

class GameField
{
private:
    Vector2Int size;
    Vector2Int padding;
    std::shared_ptr<Room> room;
    std::unique_ptr<Level> level;
    std::shared_ptr<Player> player;

public:
    bool Tick();
    void Draw() const;
    std::shared_ptr<std::vector<std::vector<bool>>> RoomGrid();
    bool IsOutOfBounds(const Vector2Int position) const;
    bool MakeFieldMove(const std::shared_ptr<FieldObject> object);
    void AddFieldObject(const std::shared_ptr<FieldObject> object);
    std::shared_ptr<FieldObject> Hit(std::shared_ptr<FieldObject> obj, Vector2Int pos) const;
    bool CheckHit(const std::shared_ptr<FieldObject> obj) const;
    bool ReachedPlayer(const std::shared_ptr<FieldObject> obj) const;
    GameField(Vector2Int size, Vector2Int padding, std::shared_ptr<Player> player);
};
