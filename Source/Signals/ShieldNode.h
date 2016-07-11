#pragma once

#include "ActionNode.h"
#include "AttackClass.h"

class ShieldNode : public ActionNode
{
public:
	ShieldNode();

	void FromXml(FXmlNode * node) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	// The type of attack being shielded.
	EAttackClass _class;

	// Duration in turns of the shield.
	int _duration;
};
