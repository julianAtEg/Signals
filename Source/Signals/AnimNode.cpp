#include "Signals.h"
#include "AnimNode.h"

static ActionNode * ctor()
{
	return new AnimNode();
}

static const FString s_type(TEXT("anim"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

AnimNode::AnimNode()
: ActionNode(s_type)
, _anim(TEXT("???"))
, _destination(AnimDestination::DontPlay)
{
}

void AnimNode::FromXml(FXmlNode const * node)
{
	_anim = node->GetAttribute(TEXT("name"));
	auto playOn = node->GetAttribute(TEXT("playOn")).ToLower();
	if (playOn == TEXT("source"))
	{
		_destination = AnimDestination::PlayOnSource;
	}
	else if (playOn == TEXT("targets"))
	{
		_destination = AnimDestination::PlayOnTargets;
	}
	else
	{
		UE_LOG(SignalsLog, Error, TEXT("Undefined anim destination '%s'"), *playOn);
	}

	ActionNode::FromXml(node);
}

void AnimNode::Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger anim '%s'"), *_anim);
}