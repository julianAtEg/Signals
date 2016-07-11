// (c) Smoke and Mirrors Games 2016

#pragma once

#include "StatType.h"
#include "ShieldInfo.h"

class UPlayerStats;
class ActionInstance;
class Action;

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

	// Paused
	Paused,
};

/**
 * Data structure holding the various components of player data in a single location.
 */
struct SIGNALS_API Combatant
{
	inline Combatant(APlayerStart * start, bool human, ACharacter * avatar, UPlayerStats * stats)
		: Start(start)
		, IsHuman(human)
		, Avatar(avatar)
		, Stats(stats)
		, TurnDelay(0)
		, State(ActionState::Idle)
		, TurnCounter(0)
		, Activity(nullptr)
		, HPDamageThisTurn(0)
		, ActionMissed(false)
		, TookDamage(false)
		, IsAlive( true )
		, Shields()
	{

	}

	// The start point for the character.
	APlayerStart * const Start;

	// Is the combatant human?
	bool IsHuman;

	// Combatant's 3D avatar.
	ACharacter * Avatar;

	// Stats for the combatant.
	UPlayerStats * const Stats;

	// Notional delay between turns for scheduler.
	int TurnDelay;

	// The current state of the combatant.
	ActionState State;

	// Counter variable used to warm up.
	int TurnCounter;

	// The action (if any) the combatant is performing.
	ActionInstance * Activity;

	// Any damage accrued this turn.
	int HPDamageThisTurn;
	bool ActionMissed;
	bool TookDamage;

	// If true, the player is alive.
	bool IsAlive;

	// Player shields.
	ShieldInfo Shields;

	// Can the player perform the supplied action?
	bool CanPerformAction(Action * const action) const;

	void OnTurnBeginning();

	// Called at begin and end of a battle.
	void BeginBattle();
	void EndBattle();
};
