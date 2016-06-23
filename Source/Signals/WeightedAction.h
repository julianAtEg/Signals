#pragma once

class Action;

/**
 * Class to hold action name and corresponding weight. Lazily returns the
 * action instance when requested.
 */
struct WeightedAction
{
	WeightedAction();
	Action * GetAction() const;
	FString ActionName;
	int Weight;

private:
	mutable Action * _action;
};

