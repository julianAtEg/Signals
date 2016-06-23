#pragma once

#include "BasicActionSelector.h"

class IntermediateActionSelector : public BasicActionSelector
{
public:
	IntermediateActionSelector() = default;
	~IntermediateActionSelector() = default;

	SelectionResult Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc) override;
};
