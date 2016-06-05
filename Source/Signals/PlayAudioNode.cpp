#include "Signals.h"
#include "PlayAudioNode.h"
#include "SignalsBattleMode.h"

//-----------------------------------------------------------------------------

static ActionNode * ctor()
{
	return new PlayAudioNode();
}

static const FString s_type(TEXT("audio"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

static void playSound(UWorld * world, AActor * object, USoundWave * sound);

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

void PlayAudioNode::PostInitialize(Action * const)
{
	// TODO: volume.
	auto name = FString::Printf(TEXT("SoundWave'/Game/Audio/%s.%s'"), *_audioFile, *_audioFile);
	_sound = LoadObject<USoundWave>(nullptr, *name, nullptr, LOAD_None, nullptr);
	if (_sound != nullptr)
	{
		_sound->AddToRoot();
	}
	else
	{
		UE_LOG(SignalsLog, Warning, TEXT("Could not locate sound resource '%s'"), *_audioFile);
	}
}

void PlayAudioNode::Execute(ASignalsBattleMode * const battle)
{
	UE_LOG(SignalsLog, Log, TEXT("Playing audio '%s'"), *_audioFile);

	auto world = battle->GetWorld();
	switch (GetDestination())
	{
		case Destination::Source:
		{
			auto source = battle->GetActionSource();
			playSound(world, source->Avatar, _sound);
			break;
		}

		case Destination::Targets:
		{
			auto & targets = battle->GetActionTargets();
			for (auto target : targets)
			{
				playSound(world, target->Avatar, _sound);
			}
			break;
		}
	}
}

//-----------------------------------------------------------------------------

static void playSound(UWorld * world, AActor * object, USoundWave * sound)
{
	if (sound != nullptr)
	{
		auto position = object->GetActorLocation();
		auto rotation = object->GetActorRotation();
		UGameplayStatics::PlaySoundAtLocation(world, sound, position, rotation);
	}
}