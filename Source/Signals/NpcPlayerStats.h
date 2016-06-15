// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PlayerStats.h"
#include "NpcPlayerStats.generated.h"

/**
 * POD to hold action name and corresponding weight.
 */
struct WeightedAction
{
	FString Action;
	int Weight;
};

/**
 * Stats subclass for NPCs.
 */
UCLASS()
class SIGNALS_API UNpcPlayerStats : public UPlayerStats
{
	GENERATED_BODY()

public:
	// The energy budget of the NPC.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Ergs;
	
	// How attack-focussed the NPC is.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Aggression;

	// At 0, randomly picks a target. At 100, picks the weakest target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Callousness;

protected:
	void fromXml(FXmlNode * const node) override;

private:
	// Weighted Actions
	TArray<WeightedAction> _weightedActions;
};

