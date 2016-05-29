#include "Signals.h"
#include "WarmupNode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new WarmupNode();
}

static const FString s_type = TEXT("warmup");
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

WarmupNode::WarmupNode()
: ActionNode(s_type)
, _turns(0)
{

}

void WarmupNode::FromXml(FXmlNode const * node)
{
	auto turnsStr = node->GetAttribute(TEXT("turns"));
	if (!turnsStr.IsEmpty())
	{
		_turns = FCString::Atoi(*turnsStr);
	}

	ActionNode::FromXml(node);
}

void WarmupNode::Execute(UWorld * world, Combatant * source, TArray<Combatant *> const &)
{
	UE_LOG(SignalsLog, Log, TEXT("Executing WARMUP"));
}

void WarmupNode::postInitialize(Action * action)
{
	action->SetWarmupRounds(_turns);
}