#pragma once

#include "ActionNode.h"

/**
 * Node subclass that invokes a named action.
 */
class RunNode : public ActionNode
{
public:
	RunNode();

	void FromXml(FXmlNode * node) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	FString _action;
};
