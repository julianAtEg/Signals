#include "Signals.h"
#include "AnimNode.h"
#include "SignalsBattleMode.h"

static ActionNode * ctor()
{
	return new AnimNode();
}

static const FString s_type(TEXT("anim"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

AnimNode::AnimNode()
: ActionNode(s_type)
, _anim(TEXT(""))
, _sound(nullptr)
{
}

AnimNode::~AnimNode()
{
	if (_sound != nullptr)
	{
		if (_sound->IsValidLowLevel())
		{
			_sound->ConditionalBeginDestroy();
			_sound = nullptr;
		}
	}
}

void AnimNode::FromXml(FXmlNode * const node)
{
	_anim = node->GetAttribute(TEXT("name"));
	auto sound = node->GetAttribute(TEXT("audio"));
	if (!sound.IsEmpty())
	{
		auto name = FString::Printf(TEXT("SoundWave'/Game/Audio/%s.%s'"), *sound, *sound);
		_sound = LoadObject<USoundWave>(nullptr, *name, nullptr, LOAD_None, nullptr);
		if (_sound != nullptr)
		{
			_sound->AddToRoot();
		}
		else
		{
			UE_LOG(SignalsLog, Warning, TEXT("Could not locate sound resource '%s'"), *sound);
		}
	}

	ActionNode::FromXml(node);
}

void AnimNode::Execute(ASignalsBattleMode * const battle)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger anim '%s'"), *_anim);

	switch (GetDestination())
	{
		case Destination::Source:
		{
			auto source = battle->GetActionSource();
			battle->PlayAnimation(source->Avatar, _anim, _sound);
			break;
		}

		case Destination::Targets:
		{
			auto & targets = battle->GetActionTargets();
			for (auto & target : targets)
			{
				battle->PlayAnimation(target->Avatar, _anim, _sound);
			}
			break;
		}
	}
}