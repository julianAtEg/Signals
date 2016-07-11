#include "Signals.h"
#include "ShieldInfo.h"

ShieldInfo::ShieldInfo()
{
	for (int i = 0; i < EAttackClass::NumAttackClasses; ++i)
	{
		_turnsLeft[i] = 0;
	}
}

void ShieldInfo::ActivateShield(EAttackClass type, int numTurns)
{
	UE_LOG(SignalsLog, Log, TEXT("ShieldInfo::ActivateShield()"));

	check(numTurns > 0);

	_turnsLeft[type] = numTurns;
}

void ShieldInfo::DeactivateShield(EAttackClass type)
{
	UE_LOG(SignalsLog, Log, TEXT("ShieldInfo::DeactivateShield()"));

	_turnsLeft[type] = 0;
}

bool ShieldInfo::Update()
{
	bool status = false;
	for (int i = 0; i < EAttackClass::NumAttackClasses; ++i)
	{
		if (_turnsLeft[i] > 0)
		{
			--_turnsLeft[i];
			if (_turnsLeft[i] == 0)
			{
				status = true;
			}
		}
	}

	return status;
}
