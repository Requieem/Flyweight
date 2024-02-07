#include "include/Food.h"

Food::Food(int healAmount) : Item(), Damage(healAmount) {}
int Food::Color() const { return PLAYER_COLOR; }
std::string Food::DisplayChar() const { return "\u0040"; }

void Food::Effect(std::shared_ptr<Player> player) const
{
	player->ChangeHealth(damage);
}
