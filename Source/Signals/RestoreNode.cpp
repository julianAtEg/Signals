#include "Signals.h"
#include "RestoreNode.h"
#include "SignalsBattleMode.h"
#include "PlayerStats.h"
#include "ActionInstance.h"
#include "Combat.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new RestoreNode(); }
static const FString s_type(TEXT("restore"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

RestoreNode::RestoreNode()
: StatNode(s_type)
{
}

void RestoreNode::PostInitialize(Action * const action)
{
	action->SetClass(EActionClass::Restorative);
	action->SetScore(GetMax());
	StatNode::PostInitialize(action);
}

void RestoreNode::executeInner(ASignalsBattleMode * battle, Combatant * target)
{
	UE_LOG(SignalsLog, Log, TEXT("RestoreNode::executeInner()"));

	int regain;
	if (!IsFixed())
	{
		auto source = battle->GetActionSource();
		auto rng = battle->GetRandom();
		auto attackStats = source->Stats;
		auto actionName = source->Activity->GetName();
		regain = attackStats->ComputeRegain(rng, GetBase(), GetLevelScale(), actionName);

		// Any boost to apply?
		auto boostFrac = battle->GetBoostFraction();
		auto boostScalar = Combat::ComputeBoostFactor(boostFrac, attackStats->Level);
		regain = (int)(float(regain)*boostScalar);
	}
	else
	{
		regain = GetBase();
	}

	auto targetStats = target->Stats;

	// TODO: handle other types of stat?
	targetStats->ApplyStatChange(EStatClass::HitPoints, regain);
	FVector green(0.25, 1, 0.05);
	auto visual = FString::Printf(TEXT("+%dHP"), regain);
	battle->AddFloatingNotification(target->Avatar, visual, green);
}