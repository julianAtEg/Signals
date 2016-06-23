#include "Signals.h"
#include "AdvancedActionSelector.h"
#include "SignalsBattleMode.h"
#include "ActionUtil.h"
#include "NpcPlayerStats.h"
#include "Random.h"
#include "StatType.h"
#include "Action.h"

//-----------------------------------------------------------------------------

static AdvancedActionSelector s_instance;
static ActionSelectionPolicy * s_policy = BasicStrategy::RegisterActionSelector(TEXT("advanced"), &s_instance);

//-----------------------------------------------------------------------------

static Action * findMaximal(UNpcPlayerStats * stats, TArray<WeightedAction> const & healers, EStatClass type, bool acceptMultiple);

//-----------------------------------------------------------------------------

SelectionResult AdvancedActionSelector::Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc)
{
	// Find my friends.
	auto friends = battle->GetActiveNPCs();
	if (friends.Num() == 1)
	{
		// Only me.
		return IntermediateActionSelector::Run(strategy, options, battle, npc);
	}

	// Do I have restorative powers?
	auto healers = ActionUtil::FindActionsOfType(options, EActionClass::Restorative);
	if (healers.Num() == 0)
	{
		// Nope. Can't help my colleagues.
		return IntermediateActionSelector::Run(strategy, options, battle, npc);
	}

	// Higher altruism means I'm more likely to help others.
	auto stats = Cast<UNpcPlayerStats>(npc->Stats);
	auto rng = battle->GetRandom();
	if (rng->NextInt(0, 100) > stats->Altruism)
	{
		// Nope, I'm going to be selfish here.
		return IntermediateActionSelector::Run(strategy, options, battle, npc);
	}

	// Anyone in need of help?
	for (auto & pal : friends)
	{
		if (selectRestorative(strategy, npc, pal, battle, healers, false))
		{
			return SelectionResult(SelectionResult::Succeeded | SelectionResult::TargetSet);
		}
	}

	// Nothing more I can do, just attend to myself.
	return IntermediateActionSelector::Run(strategy, options, battle, npc);
}

bool AdvancedActionSelector::selectRestorative
(
	BasicStrategy * strategy, 
	Combatant * npc, 
	Combatant * ally, 
	ASignalsBattleMode * battle, 
	TArray<WeightedAction> const & healers, 
	bool acceptMultiple
)
{
	auto stats = Cast<UNpcPlayerStats>(ally->Stats);
	int hpPercent = (100 * stats->HitPoints) / stats->MaxHitPoints;
	if (hpPercent < 20)
	{
		Action * best = findMaximal( stats, healers, EStatClass::HitPoints, acceptMultiple );
		if (best != nullptr)
		{
			strategy->SetAction(best);
			strategy->SetTarget(ally);
			return true;
		}
	}

	int ergPercent = (100 * stats->Ergs) / stats->MaxErgs;
	if (ergPercent < 20)
	{
		Action * best = findMaximal(stats, healers, EStatClass::Energy, acceptMultiple);
		if (best != nullptr)
		{
			strategy->SetAction(best);
			strategy->SetTarget(ally);
			return true;
		}
	}

	// TODO: statuses, shields...

	// consider healing everyone if no single option is available.
	if (!acceptMultiple)
	{
		return selectRestorative(strategy, npc, ally, battle, healers, true);
	}

	// Nope, nothing doing.
	return false;
}

//-----------------------------------------------------------------------------

static Action * findMaximal(UNpcPlayerStats * stats, TArray<WeightedAction> const & healers, EStatClass type, bool acceptMultiple)
{
	Action * best = nullptr;
	int bestScore = 0;
	auto choice = ActionUtil::FindActionWithTarget(healers, type);
	for (auto & wa : choice)
	{
		auto healer = wa.GetAction();		
		if ((healer != nullptr) && (healer->AffectsMultipleTargets() == acceptMultiple))
		{
			auto score = int((1.0f + stats->Level / 100.0f) * healer->GetScore()) - healer->GetCost();
			if (score > bestScore)
			{
				best = healer;
				bestScore = score;
			}
		}
	}

	return best;
}
