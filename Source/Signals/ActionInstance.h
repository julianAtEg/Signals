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

	// Update the current activity wrt time.
	void Update(ASignalsBattleMode * const battle, float dt);

private:
	Action * const _action;
	ContainerNode * _currentNode;
	ActionRunner _runner;
};

inline FString const & ActionInstance::GetName() const
{
	return _action->GetName();
}

inline int ActionInstance::GetWarmupTurns() const
{
	return _action->GetWarmupRounds();
}
