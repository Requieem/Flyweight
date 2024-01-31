#pragma once

#include "Item.h"
#include "Score.h"
#include "Player.h"

class Treasure : virtual public Item, public Score
{
public:
	Treasure(int score);
	int Color() const override;
	std::string DisplayChar() const override;
	void Effect(std::shared_ptr<Player> player) const override;
};