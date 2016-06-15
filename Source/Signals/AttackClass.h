#pragma once

#include "Signals.h"

UENUM(BlueprintType)
enum EAttackClass
{
	Physical,
	Fire,
	Ice,
	Electrical,
	Light,
	Plasma,
	Sound,
	Poison,
	Bacterial,
	Viral,

	// New values above here.
	NumClasses
};

namespace AttackClass
{
	EAttackClass FromString(FString const & str);
	FString ToString(EAttackClass attackClass);
}