#pragma once

#include "Strategy.h"
#include "NpcPlayerStats.h"

class Action;
class BasicStrategy;

/**
 * Result of an action selection operation.
 */
enum SelectionResult
{
	// Couldn't find an action.
	Failed = 0,

	// An action was found.
	Succeeded = 1,

	// A target was also produced as a result of selection.
	TargetSet = 2
};

/**
 * Abstract class used to pick an action to run.
 */
class ActionSelectionPolicy
{
public:
	virtual ~ActionSelectionPolicy() = default;
	virtual SelectionResult Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc) = 0;

protected:
	ActionSelectionPolicy() = default;
};

/**
 * Abstract class used to pick a viable target.
 */
class TargetSelectionPolicy
{
public:
	virtual ~TargetSelectionPolicy() = default;
	virtual bool Run(BasicStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc) = 0;

protected:
	TargetSelectionPolicy() = default;
};

/**
 * Flexible strategy for selection of an action and a target.
 */
class BasicStrategy : public Strategy
{
public:
	BasicStrategy();

	bool Run(ASignalsBattleMode * battle, Combatant * npc) override;
	void FromXml(FXmlNode * node) override;

	void SetAction(Action * action);
	void SetTarget(Combatant * target);
	void SetTargets(TArray<Combatant *> const & targets);
	Action * GetAction() const;

	static ActionSelectionPolicy * RegisterActionSelector(FString const & type, ActionSelectionPolicy * asp);
	static TargetSelectionPolicy * RegisterTargetSelector(FString const & type, TargetSelectionPolicy * tsp);

protected:
	void setActionSelector(ActionSelectionPolicy * asp);
	void setTargetSelector(TargetSelectionPolicy * tsp);

private:
	ActionSelectionPolicy * _selectAction;
	TargetSelectionPolicy * _selectTarget;
	Action * _action;
	TArray<Combatant *> _targets;
};

inline Action * BasicStrategy::GetAction() const
{
	return _action;
}