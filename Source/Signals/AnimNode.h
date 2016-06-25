#pragma once

#include "AudioNode.h"

class AnimNode : public AudioNode
{
public:
	AnimNode();

	void FromXml(FXmlNode * const node) override;
	bool Update(ASignalsBattleMode * const battle, float dt) override;

protected:
	void executeInner(ASignalsBattleMode * const battle, Combatant * combatant) override;

private:
	FString _anim;
	bool _block;
};


