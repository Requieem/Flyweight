#pragma once

class Life
{
protected:
	float startingHealth;
	float currentHealth;
public:
	virtual void ChangeHealth(float amount);
	virtual bool IsDead() const;

	Life();
	Life(float startingHealth);
};
