// (c) Smoke and Mirrors Games 2016

#pragma once

#include "PlayerStats.h"
#include "Ability.h"
#include "Curve.h"
#include "BattleSkills.h"
#include "HumanPlayerStats.generated.h"

class Random;

/**
 * Stats for a player character controlled agent.
 */
UCLASS(BlueprintType)
class SIGNALS_API UHumanPlayerStats : public UPlayerStats
{
	GENERATED_UCLASS_BODY()
public:	
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

	// Sets the initial values of all fields.
	void SetInitialValues();

	// Does the player have the given skill?
	UFUNCTION(BlueprintPure, Category = "Stats")
	bool HasSkill(BattleSkill skill);

	// Adds a skill to the player.
	void AddSkill(BattleSkill skill);

	// Finds an ability by name.
	Ability const * FindAbility(FString const & name) const;

	// Adds a new ability.
	void AddAbility(Ability const & ability);

	// Gets currently available actions for the player.
	TArray<FString> GetAvailableActionNames() const;

	int ComputeAttack(Random * rng, int base, int levelScale, FString const & action) const override;
	int ComputeRegain(Random * rng, int base, int levelScale, FString const & action) const override;

	TArray<FString> GetActions() const override;

protected:
	void fromXml(FXmlNode * const root) override;

private:
	TArray<Ability> _abilities;
	TMap< int, TArray<Ability> > _abilityLevelMap;
	Curve _levelCurve;
	Curve _hpCurve;
	int _nextExpLevel;
	TSet<BattleSkill> _skills;
};
