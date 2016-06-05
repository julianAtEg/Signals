// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PlayerStats.h"
#include "Ability.h"
#include "Curve.h"
#include "HumanPlayerStats.generated.h"

/**
 * Stats for a player character controlled agent.
 */
UCLASS(BlueprintType)
class SIGNALS_API UHumanPlayerStats : public UPlayerStats
{
	GENERATED_UCLASS_BODY()
public:	
	// Character development level. [0,99].
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Level;	

	// Experience measure.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int EXP;

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetExpToNextLevel() const;

	// Loads the immuntable data for the player.
	void LoadStaticData(FString const & playerName);

	// Called when the player level should increase.
	UFUNCTION(BlueprintCallable,Category="Stats")
	void NextLevel( TArray<FString> & newAbilities );

	// Gets currently available actions for the player.
	TArray<FString> GetAvailableActionNames() const;

	// Sets the initial values of all fields.
	void SetInitialValues();

protected:
	void fromXml(FXmlNode * const root) override;

private:
	TArray<Ability> _abilities;
	TMap< int, TArray<Ability> > _abilityLevelMap;
	Curve _levelCurve;
	Curve _hpCurve;
	int _nextExpLevel;
};
