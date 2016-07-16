// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "Combatant.h"
#include "Action.h"
#include "PlayerStats.h"

namespace
{
	struct DeactivateShieldsTask : public PlayerTask
	{
		DeactivateShieldsTask(EAttackClass shield, int interval)
		: PlayerTask(interval, 1)
		, Shield(shield)
		{

		}

		bool Run(Combatant * player) override
		{
			player->DeactivateShield(Shield);
			return true;
		}

		const EAttackClass Shield;
	};
}

Combatant::Combatant(APlayerStart * start, bool human, ACharacter * avatar, UPlayerStats * stats)
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
, IsAlive(true)
, _shields()
, _tasks()
, _status(0U)
{
	for (int i = 0; i < EAttackClass::NumAttackClasses; ++i)
	{
		_shields[i] = 0;
	}
}


void Combatant::OnTurnBeginning()
{
	HPDamageThisTurn = 0;
	ActionMissed = false;
	TookDamage = false;

	_tasks.RunPendingTasks(this);
}

bool Combatant::CanPerformAction(Action * const action) const
{
	// TODO: implement me. 
	// if (player is Human && action->Cost < gameInstance->Ergs) return true;
	// if player is npc && action->Cost < stats->Ergs) return true;
	return true;
}

void Combatant::BeginBattle()
{
	Stats->BeginBattle();
}

void Combatant::EndBattle()
{
	_tasks.Finish(this);
	Stats->EndBattle();
}

void Combatant::ActivateShield(EAttackClass type, int duration)
{
	UE_LOG(SignalsLog, Log, TEXT("Combatant::ActivateShield()"));

	_shields[type]++;
	auto task = new DeactivateShieldsTask(type, duration);
	_tasks.Schedule(task);
}

bool Combatant::DeactivateShield(EAttackClass type)
{
	UE_LOG(SignalsLog, Log, TEXT("Combatant::DeactivateShield()"));

	--_shields[type];
	checkf(_shields[type] >= 0, TEXT("Shield logic error"));
	return true;
}

void Combatant::SetStatus(EPlayerStatus status)
{
	// TODO.
}

void Combatant::ClearStatus(EPlayerStatus status)
{
	// TODO.
}

void Combatant::AddTask(PlayerTask * task)
{
	_tasks.Schedule(task);
}