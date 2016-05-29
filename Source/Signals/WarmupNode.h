#pragma once

#include "Action.h"

class WarmupNode : public ActionNode
{
public:
	WarmupNode();

	void FromXml(FXmlNode const * node) override;
	void Execute(UWorld * world, Combatant * source, TArray<Combatant *> const &) override;
	int GetWarmupTurns() const;

private:
	void postInitialize(Action * action) override;
	int _turns;
};

int WarmupNode::GetWarmupTurns() const
{
	return _turns;
}
