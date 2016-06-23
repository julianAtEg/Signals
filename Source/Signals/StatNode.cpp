#include "Signals.h"
#include "StatNode.h"
#include "StatType.h"
#include "Action.h"

StatNode::StatNode(FString const & type)
: ActionNode(type)
, _type(EStatClass::Undefined)
, _base(0)
, _levelScale(1)
, _min(0)
, _max(1)
{
}

void StatNode::FromXml(FXmlNode * const node)
{
	ActionNode::FromXml(node);

	auto typeStr = node->GetAttribute(TEXT("type"));
	_type = StatType::FromString(typeStr);

	auto baseStr = node->GetAttribute(TEXT("base"));
	_base = FCString::Atoi(*baseStr);

	auto scaleStr = node->GetAttribute(TEXT("levelScale"));
	_levelScale = FCString::Atoi(*scaleStr);

	auto minStr = node->GetAttribute(TEXT("min"));
	_min = FCString::Atoi(*minStr);

	auto maxStr = node->GetAttribute(TEXT("max"));
	_max = FCString::Atoi(*maxStr);
}

void StatNode::PostInitialize(Action * const action)
{
	action->AddTarget(_type);
	ActionNode::PostInitialize(action);
}