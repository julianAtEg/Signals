// (c) Smoke and Mirrors Games 2016

#pragma once

#include "StatType.h"
#include "PlayerTask.h"
#include "AttackClass.h"
#include "PlayerStatus.h"

class UPlayerStats;
class ActionInstance;
class Action;
class StatModifier;

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

	// Miss a turn.
	SkipTurn,
};

/**
 * Data structure holding the various components of player data in a single location.
 */
struct SIGNALS_API Combatant
{
	Combatant(APlayerStart * start, bool human, ACharacter * avatar, UPlayerStats * stats);

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

	// Can the player perform the supplied action?
	bool CanPerformAction(Action * const action) const;

	// Called when the player's turn starts.
	void OnTurnBeginning();

	// Called at begin and end of a battle.
	void BeginBattle();
	void EndBattle();

	// Shield API.
	void ActivateShield(EAttackClass type, int duration);
	bool DeactivateShield(EAttackClass type);
	bool IsShielded(EAttackClass type) const;

	// Status API
	void SetStatus(EPlayerStatus status);
	void ClearStatus(EPlayerStatus status);
	bool HasStatus(EPlayerStatus status) const;

	// Tasks.
	void AddTask(PlayerTask * task);
	PlayerSchedule & GetTaskSchedule();

private:
	int _shields[EAttackClass::NumAttackClasses];	
	PlayerSchedule _tasks;
	unsigned _status;
	TMap< EPlayerStatus, TArray<StatModifier*> > _statusStatChanges;
};

inline PlayerSchedule & Combatant::GetTaskSchedule()
{
	return _tasks;
}

inline bool Combatant::IsShielded(EAttackClass type) const
{
	return(_shields[type] > 0);
}

