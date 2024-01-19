#pragma once

class Score
{
protected:
	int score;

public:
	Score();
	Score(int startingScore);
	int GetScore() const;
	void ChangeScore(int amount);
};