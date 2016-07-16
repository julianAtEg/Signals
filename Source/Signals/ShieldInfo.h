#pragma once

#include "AttackClass.h"

class ShieldInfo
{
public:
	ShieldInfo();
	~ShieldInfo() = default;

	bool IsShielded(EAttackClass against) const;
	void ActivateShield(EAttackClass against);
	void DeactivateShield(EAttackClass against);

private:
	// The number of turns left in each type of shield.
	bool _enabled[EAttackClass::NumAttackClasses];
};

inline bool ShieldInfo::IsShielded(EAttackClass against) const
{
	return(_enabled[against]);
}