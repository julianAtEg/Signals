#pragma once

#include "WeightedAction.h"
#include "ActionClass.h"
#include "StatType.h"

class Random;

class ActionUtil
{
public:
	static TArray<WeightedAction> FindActionsOfType(TArray<WeightedAction> const & options, EActionClass type);
	static TArray<WeightedAction> FindActionWithTarget(TArray<WeightedAction> const & options, EStatClass target);
	static Action * SelectWeightedAction(Random * const rng, TArray<WeightedAction> const & options);

private:
	ActionUtil() = delete;
	~ActionUtil() = delete;
};
