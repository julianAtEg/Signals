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
{
}

void ModifyStatNode::executeInner(ASignalsBattleMode * battle, Combatant * target)
{
	auto statStr = battle->GetActionArgument(TEXT("stat"));
	auto whichStat = StatType::FromString(statStr);

	auto deltaStr = battle->GetActionArgument(TEXT("delta"));
	auto delta = FCString::Atoi(*deltaStr);

	auto transStr = battle->GetActionArgument(TEXT("transient"));
	transStr.ToLowerInline();
	auto transient = (transStr == TEXT("true"));

	target->Stats->ApplyStatChange(whichStat, delta, transient);
	auto statAbbrev = StatType::ToAbbreviatedString(whichStat);
	FVector colour;
	FString sign;
	if (delta > 0)
	{
		colour = FVector(0.2, 1, 0.1);
		sign = TEXT("+");
	}
	else
	{
		colour = FVector(1, 0.1, 0.1);
		sign = TEXT("-");
	}

	auto notStr = FString::Printf(TEXT("%s%d%s"), *sign, delta, *statAbbrev);
	battle->AddFloatingNotification(target->Avatar, notStr, colour);
	if (StatType::AffectsSchedule(whichStat))
	{
		battle->ReschedulePlayer(target);
	}
}