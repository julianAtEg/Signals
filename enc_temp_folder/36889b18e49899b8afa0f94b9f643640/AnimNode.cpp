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
: ActionNode(s_type)
, _anim(TEXT(""))
, _block(false)
, _soundMap()
{
}

AnimNode::~AnimNode()
{
}

void AnimNode::FromXml(FXmlNode * const node)
{
	_anim = node->GetAttribute(TEXT("name"));
	_soundName = node->GetAttribute(TEXT("audio"));

	// A blocking animation will pause script execution until it completes.
	auto blockStr = node->GetAttribute(TEXT("block")).ToLower();
	_block = (blockStr == TEXT("true"));

	ActionNode::FromXml(node);
}

void AnimNode::LoadResources(ASignalsBattleMode * const battle)
{
	_soundMap.Empty();
	if (!_soundName.IsEmpty())
	{
		auto resMgr = battle->GetResourceManager();
		if (_soundName[0] == '*')
		{
			// It's a character-specific sound so we have to load all the variants.
			auto stubName = _soundName.RightChop(1);
			for (auto & player : battle->GetAllCombatants())
			{
				auto playerName = player.Avatar->GetName();
				auto resName = FString::Printf(TEXT("%s%s"), *playerName, *stubName);
				auto sound = resMgr->LoadAudioResource(resName);
				if (sound != nullptr)
				{
					_soundMap.Add(playerName, sound);
				}
				else
				{
					UE_LOG(SignalsLog, Error, TEXT("Error loading sound '%s'"), *resName);
				}
			}
		}
		else
		{
			// It's a generic sound, load one for all characters.
			auto sound = resMgr->LoadAudioResource(_soundName);
			if (sound != nullptr)
			{
				for (auto & player : battle->GetAllCombatants())
				{
					auto playerName = player.Avatar->GetName();
					_soundMap.Add(playerName, sound);
				}
			}
			else
			{
				UE_LOG(SignalsLog, Error, TEXT("Error loading sound '%s'"), *_soundName);
			}
		}
	}
}

bool AnimNode::Update(ASignalsBattleMode * const, float)
{
	return !_block;
}

void AnimNode::executeInner(ASignalsBattleMode * const battle, Combatant * combatant)
{
	UE_LOG(SignalsLog, Log, TEXT("Trigger anim '%s'"), *_anim);
	if (_anim == TEXT("Punch"))
	{
		UE_LOG(SignalsLog, Log, TEXT("Break"));
	}

	USoundWave * sound = nullptr;
	if (!_soundName.IsEmpty())
	{
		auto name = combatant->Avatar->GetName();
		auto pSound = _soundMap.Find(name);
		if (pSound == nullptr)
		{
			UE_LOG(SignalsLog, Warning, TEXT("Could not locate sound '%s' for character '%s' in map"), *_soundName, *name);
		}
		else
		{
			sound = *pSound;
		}
	}

	battle->PlayAnimation(combatant->Avatar, _anim, sound, _block);
}