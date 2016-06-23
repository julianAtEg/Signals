#pragma once

#include "BasicStrategy.h"

class BasicActionSelector : public ActionSelectionPolicy
{
public:
	BasicActionSelector() = default;
	~BasicActionSelector() = default;

	SelectionResult Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc) override;
};
