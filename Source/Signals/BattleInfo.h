// (c) Smoke and Mirrors Software 2016

#pragma once

#include "Engine.h"
#include "BattleInfo.generated.h"

/**
 * POD structure to hold setup data for battles.
 */
USTRUCT(BlueprintType)
struct FBattleInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Battle Info")
	TArray<FString> OurCombatants;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle Info")
	TArray<FString> TheirCombatants;
};
