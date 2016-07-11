#pragma once

#include "ActionNode.h"

/**
 * Node used to modify a player stat. Can be transient (duration of battle) or
 * permanent. No FromXml(): this node gets its parameters from the command arguments
 * stored in the battle instance; this is to allow items to have a single action
 * to perform their magic rather than N near-identical ones.
 */
class ModifyStatNode : public ActionNode
{
public:
	ModifyStatNode();

	// No FromXml(): this node gets its parameters from the command arguments
	// stored in the battle.
protected:
	void executeInner( ASignalsBattleMode * battle, Combatant * player ) override;

private:
};