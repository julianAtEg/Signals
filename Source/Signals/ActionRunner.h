#pragma once

#include "TStack.h"

class ActionNode;
class ContainerNode;
class ASignalsBattleMode;

struct State
{
	inline State()
	: container(nullptr)
	, childIndex(-1)
	{
	}

	inline State(ContainerNode * cn, int ci)
	: container(cn)
	, childIndex(ci)
	{
	}

	inline State(State const & that)
	: container( that.container )
	, childIndex( that.childIndex )
	{
	}

	ContainerNode * container;
	int childIndex;
};

class ActionRunner
{
public:
	ActionRunner();

	// Starts running actions in the specified root container.
	void Start(ContainerNode * root);

	// Updates the runner over time.
	bool Update(ASignalsBattleMode * const battle, float dt);

	// Is the runner finished?
	bool IsFinished() const;

	// Called when an async action completes to move the execution on.
	void Advance(ASignalsBattleMode * const battle);

	// Atomically runs the command block.
	void RunAtomic(ASignalsBattleMode * battle, ContainerNode * root);

private:
	void descendInto(ContainerNode * container);

	// Current execution state.
	State _state;
	bool _newAction;

	// Execution stack.
	TStack<State> _stack;
};

inline bool ActionRunner::IsFinished() const
{
	return(_state.container == nullptr);
}
