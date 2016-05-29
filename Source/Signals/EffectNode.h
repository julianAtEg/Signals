#pragma once

#include "Action.h"

class EffectNode : public ActionNode
{
public:
	EffectNode();

	void FromXml(FXmlNode const * node) override;
	void Execute(UWorld * world, Combatant * source, TArray<Combatant *> const & targets) override;

private:
	FString _effect;
	FString _sourceSocket;
	FString _targetSocket;
};
