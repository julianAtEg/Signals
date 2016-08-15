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
		: PlayerTask(TypeID, interval, 1)
		, Shield(shield)
		{

		}

		bool Run(Combatant * player) override
		{
			player->DeactivateShield(Shield);
			return true;
		}

		const EAttackClass Shield;

		static const TaskType TypeID;
	};

	const TaskType DeactivateShieldsTask::TypeID = (const TaskType)&DeactivateShieldsTask::TypeID;
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
	_statusStatChanges.Empty();
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

bool Combatant::HasStatus(EPlayerStatus status) const
{
	return((Stats->Status & (1U << status)) != 0);
}

void Combatant::SetStatus(EPlayerStatus status)
{
	UE_LOG(SignalsLog, Log, TEXT("Combatant::SetStatus(%s)"), *PlayerStatus::ToString(status));

	if (!HasStatus(status))
	{
		// If the supplied status cancels out an existing status, just do that.
		auto antiStatus = PlayerStatus::GetAntiStatus(status);
		if ((antiStatus != EPlayerStatus::NumStatusTypes) && HasStatus(antiStatus))
		{
			ClearStatus(antiStatus);
		}
		else
		{
			// Set the new status.
			auto mask = 1u << (int)status;
			Stats->Status |= mask;
			auto result = PlayerStatus::Apply(status, this);
			_statusStatChanges.Add(status, result);
		}
	}
}

void Combatant::ClearStatus(EPlayerStatus status)
{
	UE_LOG(SignalsLog, Log, TEXT("Combatant::ClearStatus(%s)"), *PlayerStatus::ToString(status));

	if (HasStatus(status))
	{
		auto mask = 1u << (int)status;
		Stats->Status &= ~mask;	
		auto pmods = _statusStatChanges.Find(status);
		if (pmods != nullptr)
		{
			auto & mods = *pmods;
			for (auto mod : mods)
			{
				Stats->RemoveStatModifier(mod);
			}
			_statusStatChanges.Remove(status);
		}
	}
}

void Combatant::AddTask(PlayerTask * task)
{
	_tasks.Schedule(task);
}