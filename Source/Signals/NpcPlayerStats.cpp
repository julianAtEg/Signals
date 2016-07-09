// (c) Smoke and Mirrors Games 2016

#include "Signals.h"
#include "NpcPlayerStats.h"
#include "Action.h"
#include "Strategy.h"

UNpcPlayerStats::~UNpcPlayerStats()
{
	delete _strategy;
}

void UNpcPlayerStats::fromXml(FXmlNode * const node)
{
	Super::fromXml(node);

	auto ergStr = node->GetAttribute(TEXT("erg"));
	MaxErgs = Ergs = FCString::Atoi(*ergStr);

	auto aggStr = node->GetAttribute(TEXT("aggression"));
	Aggression = FCString::Atoi(*aggStr);

	auto callStr = node->GetAttribute(TEXT("callousness"));
	Callousness = FCString::Atoi(*callStr);

	auto altruismStr = node->GetAttribute(TEXT("altruism"));
	Altruism = FCString::Atoi(*altruismStr);

	auto actionsNode = node->FindChildNode(TEXT("actions"));
	check(actionsNode != nullptr);
	for (auto child : actionsNode->GetChildrenNodes())
	{
		WeightedAction action;
		action.ActionName = child->GetAttribute(TEXT("name"));
		auto weightStr = child->GetAttribute(TEXT("weight"));
		action.Weight = FCString::Atoi(*weightStr);
		_weightedActions.Add(action);
	}

	auto strategyNode = node->FindChildNode(TEXT("strategy"));
	if (strategyNode != nullptr)
	{
		auto typeStr = strategyNode->GetAttribute(TEXT("type"));
		_strategy = Strategy::Create(typeStr);
		check(_strategy != nullptr);
		_strategy->FromXml(strategyNode);
	}
	else
	{
		UE_LOG(SignalsLog, Error, TEXT("!!!NPC has no strategy"));
	}
}

TArray<FString> UNpcPlayerStats::GetActions() const
{
	TArray<FString> result;
	for (auto & wa : _weightedActions)
	{
		result.Add(wa.ActionName);
	}
	return result;
}

int UNpcPlayerStats::getEnergy() const
{
	return Ergs;
}

void UNpcPlayerStats::setEnergy(int value)
{
	Ergs = FMath::Clamp(value, 0, MaxErgs);
}
