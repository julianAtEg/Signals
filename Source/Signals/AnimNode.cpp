#include "Signals.h"
#include "AnimNode.h"
#include "SignalsBattleMode.h"
#include "ResourceManager.h"

static ActionNode * ctor()
{
	return new AnimNode();
}

static const FString s_type(TEXT("anim"));
static ActionNode::Ctor s_ctor = ActionNode::RegisterCtor(s_type, ctor);

//-----------------------------------------------------------------------------

AnimNode::AnimNode()
: AudioNode(s_type)
, _anim(TEXT(""))
, _block(false)
{
}

void AnimNode::FromXml(FXmlNode * const node)
{
	AudioNode::FromXml(node);

	_anim = node->GetAttribute(TEXT("name"));

	// A blocking animation will pause script execution until it completes.
	auto blockStr = node->GetAttribute(TEXT("block")).ToLower();
	_block = (blockStr == TEXT("true"));
}

bool AnimNode::Update(ASignalsBattleMode * const, float)
{
	return !_block;
}

void AnimNode::executeInner(ASignalsBattleMode * const battle, Combatant * combatant)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger anim '%s'"), *_anim);

	USoundWave * sound = getSound(combatant->Avatar->GetName());
	battle->PlayAnimation(combatant->Avatar, _anim, sound, _block);
}