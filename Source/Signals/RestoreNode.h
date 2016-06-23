#pragma once

#include "ActionNode.h"
#include "StatNode.h"

class RestoreNode : public StatNode
{
public:
	RestoreNode();
	void PostInitialize(Action * const action) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * target) override;
};
