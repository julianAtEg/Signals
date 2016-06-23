#pragma once

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
	NumAttackClasses
};

namespace AttackClass
{
	EAttackClass FromString(FString const & str);
	FString ToString(EAttackClass attackClass);
}