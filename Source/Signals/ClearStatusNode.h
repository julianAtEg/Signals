#pragma once

#include "ActionNode.h"
#include "PlayerStatus.h"

class ClearStatusNode 
: public ActionNode
{
public:
	ClearStatusNode();

	void FromXml(FXmlNode * node) override;

protected:
	void executeInner(ASignalsBattleMode * battle, Combatant * player) override;

private:
	TArray<EPlayerStatus> _statuses;
};
