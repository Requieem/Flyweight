#include "../include/Score.h"

Score::Score() : score(0) {}
Score::Score(int startingScore) : score(startingScore) {}

int Score::GetScore() const { return score; }
void Score::ChangeScore(int amount) { score += amount; }