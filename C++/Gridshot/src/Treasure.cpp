#include "../include/Treasure.h"

Treasure::Treasure(int score) : Item(), Score(score) {}
int Treasure::Color() const { return TREASURE_COLOR; }
std::string Treasure::DisplayChar() const { return "\u0024"; }

void Treasure::Effect(std::shared_ptr<Player> player) const
{
	player->ChangeScore(score);
}
