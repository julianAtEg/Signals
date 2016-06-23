// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PlayerStats.h"
#include "WeightedAction.h"
#include "NpcPlayerStats.generated.h"

class Strategy;

/**
 * Stats subclass for NPCs.
 */
UCLASS()
class SIGNALS_API UNpcPlayerStats : public UPlayerStats
{
	GENERATED_BODY()

public:
	~UNpcPlayerStats();

	// The energy budget of the NPC.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Ergs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int MaxErgs;

	// How attack-focussed the NPC is.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Aggression;

	// At 0, randomly picks a target. At 100, picks the weakest target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Callousness;

	// At 0, always takes selfish actions (advanced policy). At 100, always
	// try to help others.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Altruism;

	// Gets the NPC's actions and their weights.
	TArray<WeightedAction> const & GetWeightedActions() const;

	TArray<FString> GetActions() const override;

	// Gets the NPC strategy.
	Strategy * GetStrategy();

protected:
	void fromXml(FXmlNode * const node) override;

private:
	// Weighted Actions
	TArray<WeightedAction> _weightedActions;

	// NPC strategy.
	Strategy * _strategy;
};

inline TArray<WeightedAction> const & UNpcPlayerStats::GetWeightedActions() const
{
	return _weightedActions;
}

inline Strategy * UNpcPlayerStats::GetStrategy()
{
	return _strategy;
}