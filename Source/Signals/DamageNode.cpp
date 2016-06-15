#include "Signals.h"
#include "DamageNode.h"
#include "SignalsBattleMode.h"
#include "PlayerStats.h"
#include "Random.h"
#include "ActionInstance.h"
#include "Combat.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new DamageNode();
}

static ActionNode * cctor()
{
	return new DamageContainer();
}

static FString s_type(TEXT("damage"));
static FString s_ctype(TEXT("damages"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);
static ActionNode::Ctor s_cctor = ActionNode::RegisterCtor(s_ctype, cctor);

//-----------------------------------------------------------------------------

DamageContainer::DamageContainer()
: ContainerNode(s_ctype)
{

}

void DamageContainer::OnLeave(ASignalsBattleMode * const battle)
{
	UE_LOG(SignalsLog, Warning, TEXT("DamageContainer::OnLeave()"));
	battle->ApplyDamage();
}

//-----------------------------------------------------------------------------

DamageNode::DamageNode()
: StatNode(s_type)
, _class(EAttackClass::NumClasses)
, _persistent(true)
, _isAvoidable(false)
{
}

void DamageNode::FromXml(FXmlNode * const node)
{
	StatNode::FromXml(node);

	auto classStr = node->GetAttribute(TEXT("class"));
	_class = AttackClass::FromString(classStr);

	auto avoidStr = node->GetAttribute(TEXT("canMiss"));
	if (!avoidStr.IsEmpty())
	{
		_isAvoidable = (avoidStr.ToLower() == TEXT("true"));
	}
}

void DamageNode::executeInner( ASignalsBattleMode * const battle, Combatant * target )
{
	auto source = battle->GetActionSource();
	auto attackStats = source->Stats;
	auto defenceStats = target->Stats;
	auto rng = battle->GetRandom();
	if (_isAvoidable)
	{
		if (!Combat::GetHitOrMiss(rng, attackStats->Dexterity, defenceStats->Evasion))
		{
			// Oops, missed!
			UE_LOG(SignalsLog, Log, TEXT("MISSED!"));
			target->ActionMissed = true;
			return;
		}
	}

	UE_LOG(SignalsLog, Log, TEXT("DAMAGE!!!"));

	// Do the sums.
	auto actionName = source->Activity->GetName();
	int attack = attackStats->ComputeAttack(rng, GetBase(), GetLevelScale(), actionName);
	int defence = defenceStats->ComputeDefence(rng, GetLevelScale(), _class);
	int amount = FMath::Clamp( attack - defence, GetMin(), GetMax() );

	// Any boost to apply?
	auto boostFrac = battle->GetBoostFraction();
	auto boostScalar = Combat::ComputeBoostFactor(boostFrac, attackStats->Level);
	amount = (int)(float(amount)*boostScalar);

	// TODO: shields, etc, reduce the size of damage.

	// TODO: add in the type of damage done.
	target->HPDamageThisTurn += amount;
}

