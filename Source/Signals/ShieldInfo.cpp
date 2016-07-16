#include "Signals.h"
#include "ShieldInfo.h"

ShieldInfo::ShieldInfo()
{
	for (int i = 0; i < EAttackClass::NumAttackClasses; ++i)
	{
		_enabled[i] = false;
	}
}

void ShieldInfo::ActivateShield(EAttackClass type)
{
	UE_LOG(SignalsLog, Log, TEXT("ShieldInfo::ActivateShield()"));

	_enabled[type] = true;
}

void ShieldInfo::DeactivateShield(EAttackClass type)
{
	UE_LOG(SignalsLog, Log, TEXT("ShieldInfo::DeactivateShield()"));

	_enabled[type] = false;
}


