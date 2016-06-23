#pragma once

#include "ActionTarget.generated.h"

/**
* What an action is directed at. Multiple targets are possible.
*/
UENUM(BlueprintType)
enum ActionTarget
{
	// Nothing.
	None = 0,

	// Action targets HP.
	Hitpoints = 1,

	// Action targets ERG.
	Energy = 2,

	// Action targets defence (inc shields)
	Defence = 4,

	// TBD.
	Other = 8,
};

