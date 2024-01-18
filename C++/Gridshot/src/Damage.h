#pragma once

class Damage
{
protected:
	float damage = 0;
public:
	virtual float GetDamage() const;

	Damage();
	Damage(float damage);
};
