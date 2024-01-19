#pragma once

#include <memory>
#include "Player.h"

class Item
{
public:
	Item();
	virtual int Color() const = 0;
	virtual unsigned char DisplayChar() const = 0;
	virtual void Effect(std::shared_ptr<Player> player) const = 0;
};