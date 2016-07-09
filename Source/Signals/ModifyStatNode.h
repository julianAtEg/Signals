#pragma once

#include "ActionNode.h"
#include "StatType.h"

class ModifyStatNode : public ActionNode
{
public:
	ModifyStatNode();
	
	void FromXml( FXmlNode * node ) override;
	
protected:
	void executeInner( ASignalsBattleMode * battle, Combatant * player ) override;

private:
	EStatClass _whichStat;
	int _delta;
	bool _transient;
};