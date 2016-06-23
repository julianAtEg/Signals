#pragma once

#include "IntermediateActionSelector.h"

class AdvancedActionSelector : public IntermediateActionSelector
{
public:
	AdvancedActionSelector() = default;
	~AdvancedActionSelector() = default;

	SelectionResult Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc) override;

private:
	bool selectRestorative(BasicStrategy * strategy, Combatant * npc, Combatant * ally, ASignalsBattleMode * battle, TArray<WeightedAction> const & healers, bool acceptMultiple);
};
