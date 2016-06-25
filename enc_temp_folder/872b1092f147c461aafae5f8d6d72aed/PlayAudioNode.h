#pragma once

#include "ActionNode.h"

class PlayAudioNode : public ActionNode
{
public:
	PlayAudioNode();

	void FromXml(FXmlNode * const node) override;
	void LoadResources(ASignalsBattleMode* const battle) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * combatant) override;

private:
	FString _audioFile;
	USoundWave * _sound;
};