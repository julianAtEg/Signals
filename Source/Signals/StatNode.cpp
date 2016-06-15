#include "Signals.h"
#include "StatNode.h"

StatNode::StatNode(FString const & type)
: ActionNode(type)
, _type(StatType::Undefined)
, _base(0)
, _levelScale(1)
, _min(0)
, _max(1)
{
}

void StatNode::FromXml(FXmlNode * const node)
{
	ActionNode::FromXml(node);

	auto typeStr = node->GetAttribute(TEXT("type")).ToLower();
	if (typeStr == TEXT("hitpoints"))
	{
		_type = StatType::HitPoints;
	}
	else if (typeStr == TEXT("morale"))
	{
		_type = StatType::Morale;
	}
	else if (typeStr == TEXT("energy"))
	{
		_type = StatType::Energy;
	}
	else if (typeStr == TEXT("strength"))
	{
		_type = StatType::Strength;
	}
	else if (typeStr == TEXT("defence"))
	{
		_type = StatType::Defence;
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
}