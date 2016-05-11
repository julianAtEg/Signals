// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Signals.h"

class UPlayerStats;

/**
 * 
 */
struct SIGNALS_API Combatant
{
	inline Combatant(bool human, ACharacter * avatar, UPlayerStats * stats)
		: IsHuman( human )
		, Avatar( avatar )
		, Stats( stats )
	{

	}

	bool IsHuman;
	ACharacter * Avatar;
	UPlayerStats * Stats;
};
