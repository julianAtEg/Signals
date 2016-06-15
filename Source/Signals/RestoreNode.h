#pragma once

#include "ActionNode.h"
#include "StatNode.h"

class RestoreNode : public StatNode
{
public:
	RestoreNode();

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * target) override;
};
