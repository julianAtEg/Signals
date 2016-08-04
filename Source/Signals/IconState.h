#pragma once

#include "IconState.generated.h"

UENUM(BlueprintType)
enum class EIconState
{
	isIdle,
	isFadeIn,
	isShow,
	isFadeOut
};
