#pragma once

#include "BasicStrategy.h"

class BasicTargetSelector : public TargetSelectionPolicy
{
public:
	BasicTargetSelector() = default;
	~BasicTargetSelector() = default;

	bool Run(BasicStrategy * strategy, ASignalsBattleMode * battle, Combatant * npc) override;
};
