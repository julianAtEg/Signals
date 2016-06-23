#include "Signals.h"
#include "BasicActionSelector.h"
#include "SignalsBattleMode.h"
#include "Random.h"
#include "Action.h"
#include "WeightedAction.h"
#include "ActionUtil.h"

static BasicActionSelector s_instance;
static ActionSelectionPolicy * s_policy = BasicStrategy::RegisterActionSelector(TEXT("basic"), &s_instance);

SelectionResult BasicActionSelector::Run(BasicStrategy * strategy, TArray<WeightedAction> const & options, ASignalsBattleMode * battle, Combatant * npc)
{
	auto result = SelectionResult::Failed;
	if (options.Num() > 0)
	{
		auto rng = battle->GetRandom();
		auto action = ActionUtil::SelectWeightedAction(rng, options);
		check(action != nullptr);
		strategy->SetAction(action);
		result = SelectionResult::Succeeded;
	}

	return result;
}
