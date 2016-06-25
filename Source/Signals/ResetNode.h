#pragma once

#include "ActionNode.h"

// Action node to reset the player's rotation and location to the start point.
class ResetNode : public ActionNode
{
public:
	ResetNode();

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * target) override;
};
