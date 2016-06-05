#pragma once

#include "ActionNode.h"

class AnimNode : public ActionNode
{
public:
	AnimNode();
	~AnimNode();

	void FromXml(FXmlNode * const node) override;
	void Execute(ASignalsBattleMode * const battle) override;

private:
	FString _anim;
	USoundWave * _sound;
};


