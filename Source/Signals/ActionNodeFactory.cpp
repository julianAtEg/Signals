#include "Signals.h"
#include "ActionNode.h"
#include "ContainerNode.h"

//-----------------------------------------------------------------------------

static TMap<FString, ActionNode::Ctor> & ctorTable()
{
	static TMap<FString, ActionNode::Ctor> table;
	return table;
}

//-----------------------------------------------------------------------------

/*static*/
ActionNode::Ctor ActionNode::RegisterCtor(FString const & name, Ctor ctor)
{
	UE_LOG(SignalsLog, Log, TEXT("Registering ActionNode ctor for type '%s'"), *name);

	ctorTable().Add(name, ctor);
	return ctor;
}

/*static*/
ActionNode * ActionNode::Create(FString const & type)
{
	ActionNode::Ctor * pctor = ctorTable().Find(type);
	if (pctor == nullptr)
	{
		return new ContainerNode(type);
	}
	else
	{
		ActionNode::Ctor ctor = *pctor;
		return ctor();
	}
}


