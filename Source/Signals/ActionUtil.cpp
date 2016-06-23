#include "Signals.h"
#include "ActionUtil.h"
#include "Action.h"
#include "Random.h"
#include "WeightedAction.h"

/*static*/
Action * ActionUtil::SelectWeightedAction(Random * const rng, TArray<WeightedAction> const & options)
{
	// TODO: optimise to avoid doing this every time.
	TArray<int> weights;
	for (auto & wa : options)
	{
		weights.Add(wa.Weight);
	}
	WeightedSampler sampler(rng, weights);
	int index = sampler.Next();
	return options[index].GetAction();
}

/*static*/
TArray<WeightedAction> ActionUtil::FindActionsOfType(TArray<WeightedAction> const & options, EActionClass type)
{
	TArray<WeightedAction> result;
	for (auto & wa : options)
	{
		auto action = wa.GetAction();
		if (action->GetClass() == type)
		{
			result.Add(wa);
		}
	}

	return result;
}

/*static*/
TArray<WeightedAction> ActionUtil::FindActionWithTarget(TArray<WeightedAction> const & options, EStatClass target)
{
	TArray<WeightedAction> result;
	for (auto & wa : options)
	{
		auto action = wa.GetAction();
		if ((action->GetTargets() & target) != 0)
		{
			result.Add(wa);
		}
	}

	return result;
}