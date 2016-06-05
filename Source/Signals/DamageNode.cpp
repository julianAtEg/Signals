#include "Signals.h"
#include "DamageNode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new DamageNode();
}

static FString s_type(TEXT("damage"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

DamageNode::DamageNode()
: ActionNode(s_type)
, _type(DamageType::Undefined)
, _base(0)
, _levelScale(1)
, _min(0)
, _max(1)
, _persistent(true)
{
}

void DamageNode::FromXml(FXmlNode * const node)
{
	auto typeStr = node->GetAttribute(TEXT("type")).ToLower();
	if (typeStr == TEXT("hitpoints"))
	{
		_type = DamageType::HitPoints;
	}
	else if (typeStr == TEXT("morale"))
	{
		_type = DamageType::Morale;
	}
	else if (typeStr == TEXT("energy"))
	{
		_type = DamageType::Energy;
	}
	else if (typeStr == TEXT("strength"))
	{
		_type = DamageType::Strength;
	}
	else if (typeStr == TEXT("defence"))
	{
		_type = DamageType::Defence;
	}
	else
	{
		checkf(false, TEXT("Undefined damage type '%s'"), *typeStr);
	}

	auto baseStr = node->GetAttribute(TEXT("base"));
	_base = FCString::Atoi(*baseStr);

	auto scaleStr = node->GetAttribute(TEXT("levelScale"));
	_levelScale = FCString::Atoi(*scaleStr);

	auto minStr = node->GetAttribute(TEXT("min"));
	_min = FCString::Atoi(*minStr);

	auto maxStr = node->GetAttribute(TEXT("max"));
	_max = FCString::Atoi(*maxStr);

	ActionNode::FromXml(node);
}

void DamageNode::Execute(ASignalsBattleMode * const battle)
{
	UE_LOG(SignalsLog, Log, TEXT("DAMAGE!!!"));
}
