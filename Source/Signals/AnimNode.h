#pragma once

#include "ActionNode.h"

class AnimNode : public ActionNode
{
public:
	AnimNode();
	~AnimNode();

	void FromXml(FXmlNode * const node) override;
	bool Update(ASignalsBattleMode * const battle, float dt) override;

protected:
	void executeInner(ASignalsBattleMode * const battle, Combatant * combatant) override;

private:
	FString _anim;
	USoundWave * _sound;
	bool _block;
};


