// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "NpcPlayerStats.h"

void UNpcPlayerStats::fromXml(FXmlNode * const node)
{
	Super::fromXml(node);

	auto ergStr = node->GetAttribute(TEXT("erg"));
	Ergs = FCString::Atoi(*ergStr);

	auto aggStr = node->GetAttribute(TEXT("aggression"));
	Aggression = FCString::Atoi(*aggStr);

	auto callStr = node->GetAttribute(TEXT("callousness"));
	Callousness = FCString::Atoi(*callStr);

	auto actionsNode = node->FindChildNode(TEXT("actions"));
	check(actionsNode != nullptr);
	for (auto child : actionsNode->GetChildrenNodes())
	{
		WeightedAction action;
		action.Action = child->GetAttribute(TEXT("action"));
		auto weightStr = child->GetAttribute(TEXT("weight"));
		action.Weight = FCString::Atoi(*weightStr);
		_weightedActions.Add(action);
	}
}




