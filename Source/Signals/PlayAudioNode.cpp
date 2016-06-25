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
: AudioNode(s_type)
{

}

void PlayAudioNode::executeInner(ASignalsBattleMode * const battle, Combatant * combatant)
{
	auto sound = getSound(combatant->Avatar->GetName());
	if (sound != nullptr)
	{
		auto world = battle->GetWorld();
		auto object = combatant->Avatar;
		auto position = object->GetActorLocation();
		auto rotation = object->GetActorRotation();
		UGameplayStatics::PlaySoundAtLocation(world, sound, position, rotation);
	}
}

