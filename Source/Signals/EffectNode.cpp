#include "Signals.h"
#include "EffectNode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new EffectNode();
}

static const FString s_type(TEXT("effect"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

EffectNode::EffectNode()
: ActionNode(s_type)
, _effect(TEXT("???"))
, _sourceSocket(TEXT(""))
, _targetSocket(TEXT(""))
{

}

void EffectNode::FromXml(FXmlNode const * node)
{
	_effect = node->GetAttribute(TEXT("name"));
	_sourceSocket = node->GetAttribute(TEXT("sourceSocket"));
	_targetSocket = node->GetAttribute(TEXT("targetSocket"));
	ActionNode::FromXml(node);
}

void EffectNode::Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger effect '%s'"), *_effect);
}
