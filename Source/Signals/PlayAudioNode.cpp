#include "Signals.h"
#include "PlayAudioNode.h"
#include "SignalsBattleMode.h"
#include "ResourceManager.h"

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
, _sound( nullptr )
{

}

void PlayAudioNode::FromXml(FXmlNode * const node)
{
	_audioFile = node->GetAttribute(TEXT("cue"));
	ActionNode::FromXml(node);
}

void PlayAudioNode::LoadResources(ASignalsBattleMode * const battle)
{
	auto resMgr = battle->GetResourceManager();
	_sound = resMgr->LoadAudioResource(_audioFile);
}

void PlayAudioNode::executeInner(ASignalsBattleMode * const battle, Combatant * combatant)
{
	UE_LOG(SignalsLog, Log, TEXT("Playing audio '%s'"), *_audioFile);

	if (_sound != nullptr)
	{
		auto world = battle->GetWorld();
		auto object = combatant->Avatar;
		auto position = object->GetActorLocation();
		auto rotation = object->GetActorRotation();
		UGameplayStatics::PlaySoundAtLocation(world, _sound, position, rotation);
	}
}

