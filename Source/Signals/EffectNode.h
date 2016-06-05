#pragma once

#include "ActionNode.h"

class EffectNode : public ActionNode
{
public:
	EffectNode();
	~EffectNode();

	void FromXml(FXmlNode * const node) override;
	void Execute(ASignalsBattleMode * const battle) override;
	void PostInitialize(Action * const action) override;

protected:

private:
	FString _effect;
	FString _sourceSocket;
	FString _targetSocket;
	UParticleSystem * _particles;
};
