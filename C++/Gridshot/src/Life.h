#pragma once
#include <algorithm>

class Life
{
protected:
	float startingHealth;
	float currentHealth;
public:
	virtual float Health() const;
	virtual void ChangeHealth(float amount);
	virtual bool IsDead() const;

	Life();
	Life(float startingHealth);
};
