#pragma once

#include "AudioNode.h"

class PlayAudioNode : public AudioNode
{
public:
	PlayAudioNode();

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * combatant) override;
};