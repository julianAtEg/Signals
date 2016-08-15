#include "Signals.h"
#include "ModifyStatNode.h"
#include "PlayerStats.h"
#include "Combatant.h"
#include "StatType.h"
#include "SignalsBattleMode.h"
#include "PlayerTask.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new ModifyStatNode(); }
static const FString s_type(TEXT("modifyStat"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type,ctor);

//-----------------------------------------------------------------------------

namespace
{
	struct UndoStatChangeTask : public PlayerTask
	{
		UndoStatChangeTask( StatModifier * modifier, int interval )
		: PlayerTask( TypeID, interval, 1 )
		, Modifier( modifier )
		{

		}

		bool Run(Combatant * player) override
		{
			auto statPtr = player->Stats->GetStatRef(Modifier->Stat);
			statPtr->RemoveModifier(Modifier);
			return true;
		}

		StatModifier * const Modifier;

		static const TaskType TypeID;
	};

	const TaskType UndoStatChangeTask::TypeID = (const TaskType)&UndoStatChangeTask::TypeID;
}

//-----------------------------------------------------------------------------

ModifyStatNode::ModifyStatNode()
: ActionNode(s_type)
, ItemActionNode()
, _dynamic(false)
, _whichStat(EStatClass::Undefined)
, _delta(0)
, _duration(EDuration::dtNone)
, _turns(0)
, _show(true)
{
}

void ModifyStatNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto dynStr = node->GetAttribute(TEXT("dynamic"));
	_dynamic = dynStr.ToBool();
	if (_dynamic)
	{
		// Parameters are read from the battle. Nothing more to do.
		return;
	}
	
	auto statStr = node->GetAttribute(TEXT("stat"));
	_whichStat = StatType::FromString(statStr);

	auto deltaStr = node->GetAttribute(TEXT("delta"));
	_delta = FCString::Atoi(*deltaStr);

	auto durStr = node->GetAttribute(TEXT("duration"));
	_duration = ActionDuration::FromString(durStr);
	if (_duration == EDuration::dtTurns)
	{
		auto turnsStr = node->GetAttribute(TEXT("turns"));
		_turns = FCString::Atoi(*turnsStr);
		if (_turns <= 0)
		{
			UE_LOG(SignalsLog, Warning, TEXT("No / invalid 'turns' in modifyStat XML"));
			_turns = 1;
		}
	}

	auto showStr = node->GetAttribute(TEXT("show"));
	_show = showStr.ToBool();
}

void ModifyStatNode::Apply(UPlayerStats * stats)
{
	stats->ApplyStatChange(_whichStat, _delta);
}

void ModifyStatNode::Remove(UPlayerStats * stats)
{
	stats->ApplyStatChange(_whichStat, -_delta);
}

void ModifyStatNode::executeInner(ASignalsBattleMode * battle, Combatant * target)
{
	if (_dynamic)
	{
		auto statStr = battle->GetActionArgument(TEXT("stat"));
		_whichStat = StatType::FromString(statStr);

		auto deltaStr = battle->GetActionArgument(TEXT("delta"));
		_delta = FCString::Atoi(*deltaStr);

		auto durStr = battle->GetActionArgument(TEXT("duration"));
		_duration = ActionDuration::FromString(durStr);
		if (_duration == EDuration::dtTurns)
		{
			auto turnsStr = battle->GetActionArgument(TEXT("turns"));
			_turns = FCString::Atoi(*turnsStr);
		}

		auto showStr = battle->GetActionArgument(TEXT("show"));
		_show = showStr.ToBool();
	}

	switch (_duration)
	{
	case EDuration::dtNone:
		return;

	case EDuration::dtPermanent:
		target->Stats->ApplyStatChange(_whichStat, _delta);
		break;

	case EDuration::dtBattle:
	{
		auto modifier = new AddStatModifier(_whichStat,_delta);
		auto stat = target->Stats->GetStatRef(_whichStat);
		stat->AddModifier(modifier);
		break;
	}

	case EDuration::dtTurns:
		auto modifier = new AddStatModifier(_whichStat,_delta);
		auto stat = target->Stats->GetStatRef(_whichStat);
		stat->AddModifier(modifier);
		target->AddTask(new UndoStatChangeTask(modifier, _turns));
		break;
	}

	if (_show)
	{
		auto statAbbrev = StatType::ToAbbreviatedString(_whichStat);
		FVector colour;
		FString sign;
		if (_delta > 0)
		{
			colour = FVector(0.2, 1, 0.1);
			sign = TEXT("+");
		}
		else
		{
			colour = FVector(1, 0.1, 0.1);
			sign = TEXT("-");
		}
		auto notStr = FString::Printf(TEXT("%s%d%s"), *sign, _delta, *statAbbrev);
		battle->AddFloatingNotification(target->Avatar, notStr, colour);
	}

	if (StatType::AffectsSchedule(_whichStat))
	{
		battle->ReschedulePlayer(target);
	}
}