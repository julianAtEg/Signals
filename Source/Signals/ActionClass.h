#pragma once

#include "ActionClass.generated.h"

/**
* Describes the general intent of an action.
*/
UENUM(BlueprintType)
enum EActionClass
{
	// Action is intended to reduce an enemy's trait.
	Offensive,

	// Action is intended to prevent a trait being reduced
	Defensive,

	// Action restores some trait.
	Restorative,

	// Action doesn't affect traits.
	Neutral,

	// New classes above here. Don't forget to edit the text names in
	// the cpp file!
	NumActionClasses
};

namespace ActionClass
{
	EActionClass FromString(FString const & str);
	FString ToString(EActionClass attackClass);
}