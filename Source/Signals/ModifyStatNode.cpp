#include "Signals.h"
#include "ModifyStatNode.h"
#include "PlayerStats.h"
#include "Combatant.h"
#include "StatType.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new ModifyStatNode(); }
static const FString s_type(TEXT("modifyStat"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type,ctor);

//-----------------------------------------------------------------------------

ModifyStatNode::ModifyStatNode()
: ActionNode(s_type)
, _delta(0)
, _transient(false)
{
}

void ModifyStatNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	auto statStr = node->GetAttribute(TEXT("stat"));
	_whichStat = StatType::FromString(statStr);

	auto deltaStr = node->GetAttribute(TEXT("delta"));
	_delta = FCString::Atoi(*deltaStr);

	auto transStr = node->GetAttribute(TEXT("transient"));
	transStr.ToLowerInline();
	_transient = (transStr == TEXT("true"));
}

void ModifyStatNode::executeInner(ASignalsBattleMode * battle, Combatant * target)
{
	target->Stats->ApplyStatChange(_whichStat, _delta, _transient);
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

	auto statStr = FString::Printf(TEXT("%s%d%s"), *sign, _delta, *statAbbrev);
	battle->AddFloatingNotification(target->Avatar, statStr, colour);
	if (StatType::AffectsSchedule(_whichStat))
	{
		battle->ReschedulePlayer(target);
	}
}