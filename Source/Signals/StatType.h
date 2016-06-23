#pragma once

enum EStatClass
{
	// ???
	Undefined = 0,

	// Damages HP
	HitPoints = 1,

	// Damages morale / speed
	Morale = 2,

	// Affects energy level
	Energy = 4,

	// Affects strength
	Strength = 8,

	// Affects defence
	Defence = 16,
};

namespace StatType
{
	EStatClass FromString(FString const & text);
	FString ToString(EStatClass state);
}