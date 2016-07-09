#include "Signals.h"
#include "RunNode.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor() { return new RunNode(); }
static FString s_type(TEXT("run"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

RunNode::RunNode()
: ActionNode(s_type)
{

}

void RunNode::FromXml(FXmlNode * node)
{
	_action = node->GetAttribute(TEXT("action"));
}

void RunNode::executeInner(ASignalsBattleMode * battle, Combatant * )
{
	battle->InvokeAction(_action);
}