#pragma once

#include "ActionTarget.generated.h"

/**
* What an action is directed at. Multiple targets are possible.
*/
UENUM(BlueprintType)
enum ActionTarget
{
	// Nothing.
	atNone = 0,

	// Action targets HP.
	atHitpoints = 1,

	// Action targets ERG.
	atEnergy = 2,

	// Action targets defence (inc shields)
	atDefence = 4,

	// TBD.
	atOther = 8,
};

