#pragma once

#include "ContainerNode.h"

class WarmupNode : public ContainerNode
{
public:
	WarmupNode();

	void FromXml(FXmlNode * const node) override;
	int GetWarmupTurns() const;
	void PostInitialize(Action * const action) override;

private:
	int _turns;
};

int WarmupNode::GetWarmupTurns() const
{
	return _turns;
}
