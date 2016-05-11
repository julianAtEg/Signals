#pragma once

struct Combatant;
class ActionInstance;
class UWorld;

struct Action
{
	// The name of the action.
	const char * Name;

	// The number of turns it takes to warmup before it can execute.
	int WarmupTurns;

	// The name of the warmup executor.
	const char * WarmupName;

	// The name of the action executor.
	const char * ExecuteName;

	// The name of the payload executor.
	const char * PayloadName;

	// The cost in erg of the action.
	int Cost;
};

// Signature for a function that performs part of an action.
typedef void(*Executor)(UWorld * world, ActionInstance * action, Combatant * combatant);

class ActionInstance
{
public:
	~ActionInstance();

	void RunWarmup(UWorld * world, Combatant * combatant);
	void RunAction(UWorld * world, Combatant * combatant);
	void RunPayload(UWorld * world, Combatant * combatant);

	int GetWarmupTurns() const;
	const char * GetName() const;
	int GetCost() const;

	// Factory interface, use this, no the ctor.
	static ActionInstance * Create(const char * name);

private:
	ActionInstance(Action * const action);

	Action * const _action;
	Executor const _warmup;
	Executor const _activity;
	Executor const _payload;
};

inline int ActionInstance::GetWarmupTurns() const
{
	return _action->WarmupTurns;
}

inline const char * ActionInstance::GetName() const
{
	return _action->Name;
}

inline int ActionInstance::GetCost() const
{
	return _action->Cost;
}

//-----------------------------------------------------------------------------

// API for registering actions and their executive components.
void RegisterAction(Action * action);
void RegisterExecutor(const char * name, Executor exec);
