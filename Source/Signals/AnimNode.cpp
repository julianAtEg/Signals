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
, _block(false)
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

	// A blocking animation will pause script execution until it completes.
	auto blockStr = node->GetAttribute(TEXT("block")).ToLower();
	_block = (blockStr == TEXT("true"));

	ActionNode::FromXml(node);
}

bool AnimNode::Update(ASignalsBattleMode * const, float)
{
	return !_block;
}

void AnimNode::executeInner( ASignalsBattleMode * const battle, Combatant * combatant )
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger anim '%s'"), *_anim);

	battle->PlayAnimation(combatant->Avatar, _anim, _sound);
}