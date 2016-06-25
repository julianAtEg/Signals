#pragma once

#include "Action.h"
#include "ActionRunner.h"

class Action;
class ASignalsBattleMode;
class ContainerNode;

class ActionInstance
{
public:
	ActionInstance(Action * action);

	int GetWarmupTurns() const;
	void RunWarmup(ASignalsBattleMode * const battle);
	void RunActivity(ASignalsBattleMode * const battle);
	void RunPayload(ASignalsBattleMode * const battle);
	FString const & GetName() const;
	Action * GetAction() const;

	// Update the current activity wrt time.
	void Update(ASignalsBattleMode * const battle, float dt);

	// Called when an async action has completed.
	void NotifyActionComplete(ASignalsBattleMode * const battle);

	// Has the action completed?
	bool IsFinished() const;

private:
	Action * const _action;
	ContainerNode * _currentNode;
	ActionRunner _runner;
};

inline Action * ActionInstance::GetAction() const
{
	return _action;
}

inline bool ActionInstance::IsFinished() const
{
	return(_currentNode == nullptr);
}

inline FString const & ActionInstance::GetName() const
{
	return _action->GetName();
}

inline int ActionInstance::GetWarmupTurns() const
{
	return _action->GetWarmupRounds();
}
