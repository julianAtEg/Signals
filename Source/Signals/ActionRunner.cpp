#include "Signals.h"
#include "ActionRunner.h"
#include "ActionNode.h"
#include "ContainerNode.h"

ActionRunner::ActionRunner()
: _state()
, _newAction(true)
, _stack()
{

}

void ActionRunner::Start(ContainerNode * root)
{
	_stack.Clear();
	_state.container = nullptr;
	_state.childIndex = 0;
	descendInto(root);
}

bool ActionRunner::Update( ASignalsBattleMode * const battle, float dt )
{
	while (!IsFinished())
	{
		auto action = _state.container->GetChild(_state.childIndex);
		if (action->IsContainer)
		{
			// The current action is a container.
			auto container = (ContainerNode *)action;
			if (container->GetChildCount() > 0)
			{
				// It contains things to do, so let's go look.
				descendInto(container);
			}
			else
			{
				// Nothing to do in the empty container, so just move on.
				advance();
			}
		}
		else
		{
			// It's a runnable action.
			if (_newAction)
			{
				action->Execute(battle);
				_newAction = false;
			}

			if (action->Update(battle, dt))
			{
				// Return of 'true' means it'a completed, so move on.
				advance();
			}
			else
			{
				// Action is waiting to complete, so we have to pause here.
				break;
			}
		}
	}

	return IsFinished();
}

void ActionRunner::descendInto(ContainerNode * container)
{
	_stack.Push( _state );
	_state.container = container;
	_state.childIndex = 0;
	_newAction = true;
}

void ActionRunner::advance()
{
	// Move to the next command.
	++_state.childIndex;
	_newAction = true;
	if (_state.childIndex >= _state.container->GetChildCount())
	{
		// There is no next command in this container, so try to ascend.
		if (!_stack.IsEmpty())
		{
			_state = _stack.Pop();

			// Skip past the container.
			++_state.childIndex;
		}
	}
}
