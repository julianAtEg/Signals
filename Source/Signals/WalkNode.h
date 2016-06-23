#pragma once

#include "ActionNode.h"

class WalkNode : public ActionNode
{
public:
	WalkNode();

	void FromXml(FXmlNode * node) override;
	bool Update(ASignalsBattleMode * const battle, float dt) override;

protected:
	void executeInner(ASignalsBattleMode * const battle, Combatant * player) override;

private:
	float _tolerance;
};

