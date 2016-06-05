#include "Signals.h"
#include "WarmupNode.h"
#include "Action.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new WarmupNode();
}

static const FString s_type = TEXT("warmup");
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

WarmupNode::WarmupNode()
: ContainerNode(s_type)
, _turns(0)
{

}

void WarmupNode::FromXml(FXmlNode * const node)
{
	auto turnsStr = node->GetAttribute(TEXT("turns"));
	if (!turnsStr.IsEmpty())
	{
		_turns = FCString::Atoi(*turnsStr);
	}

	ContainerNode::FromXml(node);
}

void WarmupNode::PostInitialize(Action * const action)
{
	action->SetWarmupRounds(_turns);
	ContainerNode::PostInitialize(action);
}