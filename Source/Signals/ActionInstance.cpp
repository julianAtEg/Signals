#include "Signals.h"
#include "ActionInstance.h"
#include "Action.h"

ActionInstance::ActionInstance(Action * action)
: _action(action)
, _currentNode(nullptr)
, _runner()
{
}

void ActionInstance::RunWarmup(ASignalsBattleMode * const battle)
{
	_currentNode = _action->GetWarmupNode();
	if (_currentNode != nullptr)
	{
		_runner.Start(_currentNode);
	}
}

void ActionInstance::RunActivity(ASignalsBattleMode * const battle)
{
	_currentNode = _action->GetActivityNode();
	if (_currentNode != nullptr)
	{
		_runner.Start(_currentNode);
	}
}

void ActionInstance::RunPayload(ASignalsBattleMode * const battle)
{
	_currentNode = _action->GetPayloadNode();
	if (_currentNode != nullptr)
	{
		_runner.Start(_currentNode);
	}
}

void ActionInstance::Update(ASignalsBattleMode * const battle, float dt)
{
	if (_currentNode != nullptr)
	{
		if (_runner.Update(battle, dt))
		{
			_currentNode = nullptr;
		}
	}
}
