#include "Signals.h"
#include "IntermediateActionSelector.h"
#include "SignalsBattleMode.h"
#include "NpcPlayerStats.h"
#include "Random.h"
#include "ActionUtil.h"

static IntermediateActionSelector s_instance;
static ActionSelectionPolicy * s_policy = BasicStrategy::RegisterActionSelector(TEXT("intermediate"), &s_instance);

SelectionResult IntermediateActionSelector::Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc)
{
	SelectionResult result;
	auto random = battle->GetRandom();
	auto stats = Cast<UNpcPlayerStats>(npc->Stats);

	// If I'm aggressive, chances are I'll do something that does damage to the enemy rather than heals me.
	if (random->NextInt(0, 100) < stats->Aggression)
	{
		auto offensives = ActionUtil::FindActionsOfType(options,EActionClass::Offensive);
		if (offensives.Num() > 0)
		{
			result = BasicActionSelector::Run(strategy, offensives, battle, npc);
			if (result != SelectionResult::Failed)
			{
				return result;
			}
		}
	}

	// Am I in need of defence or restoration?
	auto hpPercent = (100 * stats->HitPoints) / stats->MaxHitPoints;
	if (hpPercent < 20)
	{
		// If I'm aggressive, use defence rather than heal.
		if (random->NextInt(0, 100) > stats->Aggression)
		{
			auto defensives = ActionUtil::FindActionsOfType(options, EActionClass::Defensive);
			if (defensives.Num() > 0)
			{
				result = BasicActionSelector::Run(strategy, defensives, battle, npc);
				if (result != SelectionResult::Failed)
				{
					return result;
				}
			}
		}

		auto healers = ActionUtil::FindActionsOfType(options, EActionClass::Restorative);
		if (healers.Num() > 0)
		{
			result = BasicActionSelector::Run(strategy, healers, battle, npc);
			if (result != SelectionResult::Failed)
			{
				strategy->SetTarget(npc);
				result = (SelectionResult)(SelectionResult::Succeeded |SelectionResult::TargetSet);
				return(result);
			}
		}
	}

	return BasicActionSelector::Run(strategy, options, battle, npc);
}