#pragma once

#include "ActionNode.h"

class IndicatorNode : public ActionNode
{
public:
	IndicatorNode();

	void FromXml(FXmlNode * node) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	FString _text;
	FVector _colour;
};
