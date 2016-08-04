#pragma once

#include "ActionNode.h"
#include "PlayerStatus.h"
#include "ActionDuration.h"

class SetStatusNode : public ActionNode
{
public:
	SetStatusNode();

	void FromXml(FXmlNode * node) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	EPlayerStatus _status;
	EDuration _duration;
	int _turns;
};
