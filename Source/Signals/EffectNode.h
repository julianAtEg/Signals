#pragma once

#include "ActionNode.h"

class EffectNode : public ActionNode
{
public:
	EffectNode();
	~EffectNode();

	void FromXml(FXmlNode * const node) override;
	void PostInitialize(Action * const action) override;

protected:
	void executeInner(ASignalsBattleMode * const battle, Combatant * combatant) override;

private:
	FString _effect;
	FString _sourceSocket;
	FString _targetSocket;
	UParticleSystem * _particles;
};
