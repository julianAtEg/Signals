// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PlayerStats.h"
#include "HumanPlayerStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SIGNALS_API UHumanPlayerStats : public UPlayerStats
{
	GENERATED_UCLASS_BODY()
public:	
	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetDisplayHitPoints() const;

	// Call from tick.
	void Update(float dt);

	// Character development level. [0,99].
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Level;

private:
	int _displayHitPoints;	
};
