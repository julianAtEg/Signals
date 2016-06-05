#pragma once

#include "ActionNode.h"

class PlayAudioNode : public ActionNode
{
public:
	PlayAudioNode();

	void FromXml(FXmlNode * const node) override;
	void Execute(ASignalsBattleMode * const battle) override;
	void PostInitialize(Action * const) override;

private:
	FString _audioFile;
	USoundWave * _sound;
};