// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Signals.h"

class UPlayerStats;
class ActionInstance;

// What a character is currently doing.
enum class ActionState
{
	// Doing nothing.
	Idle,

	// Waiting for a UI command.
	Waiting,

	// Start the action.
	Start,

	// Performing an optional action warmup.
	Warmup,

	// Running an action.
	Running,

	// Action complete.
	Complete,
};


/**
 * Data structure holding the various components of player data in a single location.
 */
struct SIGNALS_API Combatant
{
	inline Combatant(bool human, ACharacter * avatar, UPlayerStats * stats)
		: IsHuman( human )
		, Avatar( avatar )
		, Stats( stats )
		, TurnDelay( 0 )
		, State( ActionState::Idle )
	{

	}

	// Is the combatant human?
	bool IsHuman;

	// Combatant's 3D avatar.
	ACharacter * Avatar;

	// Stats for the combatant.
	UPlayerStats * Stats;

	// Notional delay between turns for scheduler.
	int TurnDelay;

	// The current state of the combatant.
	ActionState State;

	// Counter variable used to warm up.
	int TurnCounter;

	// The action (if any) the combatant is performing.
	ActionInstance * Activity;
};
