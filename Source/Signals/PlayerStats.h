// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Signals.h"
#include "PlayerStats.generated.h"

/**
 * Base class for human and enemy stat APIs.
 */
UCLASS()
class SIGNALS_API UPlayerStats : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	// Health level.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int HitPoints;

	// Maximum health level at this rank.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxHitPoints;

	// This is the currency of the game. Needs fleshing out.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Energy;

	// Determines how frequently the agent gets a turn in battle. Max out at 99.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Speed;

	// Modifier to give a random boost to battle stats. Max out at 10.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Fortune;
};
