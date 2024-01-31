#pragma once

#include "Item.h"
#include "Player.h"

class Food : public Item, public Damage
{
public:
	Food(int healAmount);
	int Color() const override;
	std::string DisplayChar() const override;
	void Effect(std::shared_ptr<Player> player) const override;
};