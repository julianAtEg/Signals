#pragma once

#include "ActionNode.h"
#include "ItemActionNode.h"
#include "StatType.h"

/**
 * Node used to modify a player stat. Can be transient (duration of battle) or
 * permanent, or related to an item.
 */
class ModifyStatNode : public ActionNode, public ItemActionNode
{
public:
	ModifyStatNode();

	void FromXml(FXmlNode * node) override;
	void Apply(UPlayerStats * stats) override;
	void Remove(UPlayerStats * stats) override;

protected:
	void executeInner( ASignalsBattleMode * battle, Combatant * player ) override;

private:
	enum EDuration
	{
		dtNone,			// Undefined
		dtPermanent,	// Lasts forever
		dtBattle,		// Lasts for the duration of the battle
		dtTurns,		// Lasts for a number of turns.
	};

	bool _dynamic;
	EStatClass _whichStat;
	int _delta;
	EDuration _duration;
	int _turns;
	bool _show;
};