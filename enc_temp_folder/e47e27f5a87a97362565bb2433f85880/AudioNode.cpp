#include "Signals.h"
#include "AudioNode.h"
#include "SignalsBattleMode.h"
#include "ResourceManager.h"

AudioNode::AudioNode(FString const & type)
: ActionNode(type)
, _soundName(TEXT("???"))
, _soundMap()
{
}

USoundWave * AudioNode::getSound( FString const & name ) const
{
	auto psound = _soundMap.Find(name);
	if (psound != nullptr)
	{
		return *psound;
	}
	else
	{
		return nullptr;
	}
}

void AudioNode::FromXml(FXmlNode * node)
{
	ActionNode::FromXml(node);

	_soundName = node->GetAttribute(TEXT("cue"));
}

void AudioNode::LoadResources(ASignalsBattleMode * const battle)
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
				auto resName = FString::Printf(TEXT("%s-%s"), *playerName, *stubName);
				auto sound = resMgr->LoadAudioResource(resName);
				if (sound == nullptr)
				{
					// No specialised sound exists, try for the generic one.
					sound = resMgr->LoadAudioResource(stubName);
				}

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