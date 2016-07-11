#pragma once

#include "AttackClass.h"

class ShieldInfo
{
public:
	ShieldInfo();
	~ShieldInfo() = default;

	bool IsShielded(EAttackClass against) const;
	void ActivateShield(EAttackClass against, int numTurns);
	void DeactivateShield(EAttackClass against);
	bool Update();

private:
	// The number of turns left in each type of shield.
	int _turnsLeft[EAttackClass::NumAttackClasses];
};

inline bool ShieldInfo::IsShielded(EAttackClass against) const
{
	return(_turnsLeft[against] > 0);
}