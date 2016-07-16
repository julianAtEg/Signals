#include "Signals.h"
#include "DamageNode.h"
#include "SignalsBattleMode.h"
#include "PlayerStats.h"
#include "Random.h"
#include "ActionInstance.h"
#include "Combat.h"
#include "Action.h"
#include "Shields.h"

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

void DamageContainer::PostInitialize(Action * const action)
{
	action->SetClass(EActionClass::Offensive);

	int averageDamage = 0;
	int numKids = GetChildCount();
	if (numKids > 0)
	{
		for (int i = 0; i < GetChildCount(); ++i)
		{
			auto damageNode = (DamageNode *)GetChild(i);
			averageDamage += damageNode->GetMax();
		}

		averageDamage /= numKids;
	}
	action->SetScore(averageDamage);

	ContainerNode::PostInitialize(action);
}

//-----------------------------------------------------------------------------

DamageNode::DamageNode()
: StatNode(s_type)
, _class(EAttackClass::NumAttackClasses)
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
			// TODO: trigger DODGE animation.
			UE_LOG(SignalsLog, Log, TEXT("MISSED!"));
			target->ActionMissed = true;
			return;
		}
	}

	// Do the sums.
	int amount;
	if (!IsFixed())
	{
		auto actionName = source->Activity->GetName();
		int attack = attackStats->ComputeAttack(rng, GetBase(), GetLevelScale(), actionName);
		int defence = defenceStats->ComputeDefence(rng, GetLevelScale(), _class);
		amount = FMath::Clamp(attack - defence, GetMin(), GetMax());

		// Any boost to apply?
		auto boostFrac = battle->GetBoostFraction();
		auto boostScalar = Combat::ComputeBoostFactor(boostFrac, attackStats->Level);
		amount = (int)(float(amount)*boostScalar);
	}
	else
	{
		amount = GetBase();
	}

	if (target->IsShielded(_class))
	{
		auto shieldScalar = defenceStats->ComputeShieldFactor(rng);
		amount = int(float(amount)*shieldScalar);
		Shields::ShowShield(battle, target, _class);
	}

	// TODO: add in the type of damage done.
	UE_LOG(SignalsLog, Log, TEXT("DAMAGE = %d"), amount);
	target->HPDamageThisTurn += amount;
	target->ActionMissed = false;
	target->TookDamage = true;
}

