#include "Signals.h"
#include "PlayAudioNode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new PlayAudioNode();
}

static const FString s_type(TEXT("audio"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

PlayAudioNode::PlayAudioNode()
: ActionNode(s_type)
, _audioFile(TEXT("???"))
{

}

void PlayAudioNode::FromXml(FXmlNode const * node)
{
	_audioFile = node->GetAttribute(TEXT("file"));
	ActionNode::FromXml(node);
}

void PlayAudioNode::Execute(UWorld * world, Combatant * source, TArray<Combatant *> const &)
{
	UE_LOG(SignalsLog, Log, TEXT("Playing audio '%s'"), *_audioFile);
}