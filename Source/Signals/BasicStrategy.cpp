#include "Signals.h"
#include "BasicStrategy.h"
#include "Combatant.h"
#include "NpcPlayerStats.h"
#include "Random.h"
#include "SignalsBattleMode.h"
#include "Action.h"
#include "ActionInstance.h"
#include "WeightedAction.h"

//-----------------------------------------------------------------------------

static Strategy * ctor() { return new BasicStrategy(); }
static const FString s_type = TEXT("basic");
static Strategy::Ctor s_ctor = Strategy::RegisterCtor(s_type, ctor);

typedef TMap<FString, ActionSelectionPolicy*> ASPMap;
typedef TMap<FString, TargetSelectionPolicy*> TSPMap;
static ASPMap & aspMap();
static TSPMap & tspMap();

//-----------------------------------------------------------------------------

/*static*/
ActionSelectionPolicy * BasicStrategy::RegisterActionSelector(FString const & type, ActionSelectionPolicy * asp)
{
	UE_LOG(SignalsLog, Log, TEXT("Registered action selection policy '%s'"), *type);

	aspMap().Add(type, asp);
	return asp;
}

/*static*/
TargetSelectionPolicy * BasicStrategy::RegisterTargetSelector(FString const & type, TargetSelectionPolicy * tsp)
{
	UE_LOG(SignalsLog, Log, TEXT("Registered target selection policy '%s'"), *type);

	tspMap().Add(type, tsp);
	return tsp;
}

BasicStrategy::BasicStrategy()
: Strategy()
, _selectAction( nullptr )
, _selectTarget( nullptr )
, _action( nullptr )
{
}

void BasicStrategy::SetAction(Action * action)
{
	check(action != nullptr);
	_action = action;
}

void BasicStrategy::SetTarget(Combatant * target)
{
	_targets.Add(target);
}

void BasicStrategy::SetTargets(TArray<Combatant *> const & targets)
{
	_targets.Append(targets);
}

void BasicStrategy::FromXml(FXmlNode * node)
{
	auto asNode = node->FindChildNode(TEXT("action-selector"));
	if (asNode != nullptr)
	{
		auto asName = asNode->GetAttribute(TEXT("type"));
		auto pas = aspMap().Find(asName);
		if (pas == nullptr)
		{
			UE_LOG(SignalsLog, Error, TEXT("Could not find action selector '%s'"), *asName);
		}
		else
		{
			_selectAction = *pas;
		}
	}

	auto tsNode = node->FindChildNode(TEXT("target-selector"));
	if (tsNode != nullptr)
	{
		auto tsName = tsNode->GetAttribute(TEXT("type"));
		auto pts = tspMap().Find(tsName);
		if (pts == nullptr)
		{
			UE_LOG(SignalsLog, Error, TEXT("Could not find target selector '%s'"), *tsName);
		}
		else
		{
			_selectTarget = *pts;
		}
	}
}

bool BasicStrategy::Run(ASignalsBattleMode * battle, Combatant * npc)
{
	if (_selectAction == nullptr || _selectTarget == nullptr)
	{
		UE_LOG(SignalsLog, Warning, TEXT("Unable to run action selection"));
		return false;
	}

	_targets.Empty();
	_action = nullptr;

	// Find the valid actions for the player.
	auto stats = Cast<UNpcPlayerStats>(npc->Stats);
	auto & weightedActions = stats->GetWeightedActions();
	TArray<WeightedAction> validActions;
	for (auto & wa : weightedActions)
	{
		auto action = wa.GetAction();
		if (npc->CanPerformAction(action))
		{
			validActions.Add(wa);
		}
	}

	if (validActions.Num() == 0)
	{
		return false;
	}

	// Pick an action.
	auto rc = _selectAction->Run(this, validActions, battle, npc);
	if (rc == SelectionResult::Failed)
	{
		return false;
	}

	if (_action == nullptr)
	{
		UE_LOG(SignalsLog, Error, TEXT("!!!null action?"));
		return false;
	}

	// Did picking the action pick the target?
	if ((rc & SelectionResult::TargetSet) == 0)
	{
		// No, so pick a target or targets.
		if (!_selectTarget->Run(this, battle, npc))
		{
			UE_LOG(SignalsLog, Error, TEXT("!!!Could not selected action"));
			return false;
		}
	}

	auto actionInst = new ActionInstance(_action);
	battle->SetActionTargets(_targets);
	battle->SetCurrentCombatantAction(actionInst);

	return true;
}

void BasicStrategy::setActionSelector(ActionSelectionPolicy * asp)
{
	_selectAction = asp;
}

void BasicStrategy::setTargetSelector(TargetSelectionPolicy * tsp)
{
	_selectTarget = tsp;
}

//-----------------------------------------------------------------------------

static ASPMap & aspMap()
{
	static ASPMap map;
	return map;
}

static TSPMap & tspMap()
{
	static TSPMap map;
	return map;
}
