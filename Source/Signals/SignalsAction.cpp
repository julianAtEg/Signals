#include "Signals.h"
#include "SignalsAction.h"

//-----------------------------------------------------------------------------

static TMap<FString, Action *> s_actions;
static TMap<FString, Executor> s_execs;

//-----------------------------------------------------------------------------

void RegisterAction(Action * action)
{
	check(action != nullptr);
	FString name = FString(action->Name);
	checkf(s_actions[name] == nullptr, TEXT("Duplicate action"), *name);
	s_actions.Add(name, action);
}

void RegisterExecutor(const char * name, Executor exec)
{
	check(exec != nullptr);
	FString strName(name);
	checkf(s_execs[strName] == nullptr, TEXT("Duplicate executor"), *strName);
	s_execs.Add(strName, exec);
}

//-----------------------------------------------------------------------------

/*static*/
ActionInstance * ActionInstance::Create(const char * name)
{
	FString strName(name);
	auto action = s_actions[*strName];
	checkf(action != nullptr, TEXT("No such action '%s'"), *strName);
	return new ActionInstance(action);
}

ActionInstance::ActionInstance(Action * const action)
: _action( action )
, _warmup( nullptr )
, _activity( nullptr )
, _payload( nullptr )
{
	checkf(action != nullptr, TEXT("Null action"));

	// TODO: fixup of execs.	
}

ActionInstance::~ActionInstance()
{
}

void ActionInstance::RunWarmup(UWorld * world, Combatant * combatant)
{
	if (_warmup != nullptr)
	{
		_warmup(world, this, combatant);
	}
}

void ActionInstance::RunAction(UWorld * world, Combatant * combatant)
{
	_activity(world, this, combatant);
}

void ActionInstance::RunPayload(UWorld * world, Combatant * combatant)
{
	if (_payload != nullptr)
	{
		_payload(world, this, combatant);
	}
}