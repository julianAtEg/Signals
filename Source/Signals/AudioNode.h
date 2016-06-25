#pragma once

#include "ActionNode.h"

// Abstract class for that handles per-player audio variants.
class AudioNode : public ActionNode
{
public:
	void FromXml(FXmlNode * node) override;
	void LoadResources(ASignalsBattleMode* const battle) override;

protected:
	AudioNode(FString const & type);

	USoundWave * getSound(FString const & name) const;

private:
	FString _soundName;
	TMap<FString, USoundWave *> _soundMap;
};
