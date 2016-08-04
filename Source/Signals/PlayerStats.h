// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Ability.h"
#include "AttackClass.h"
#include "StatType.h"
#include "Stat.h"
#include "PlayerStats.generated.h"

class Random;
class ASignalsBattleMode;

/**
 * Base class for human and enemy stat APIs.
 */
UCLASS()
class SIGNALS_API UPlayerStats : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	// Character development level. [1,100].
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Level;

	// Index of the icon in the UI.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int IconIndex;

	// Status flags (see PlayerStatus.h)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Status;

	// Loads stats data from a file.
	void Load( FString const & filePath );

	// Computes attack, defence and healing values.
	virtual int ComputeAttack(Random * rng, int base, int levelScale, FString const & action) const;
	virtual int ComputeDefence(Random * rng, int levelScale, EAttackClass actionClass) const;
	virtual int ComputeRegain(Random * rng, int base, int levelScale, FString const & action) const;
	virtual float ComputeShieldFactor(Random * rng) const;

	// Gets all the known actions for the player
	virtual TArray<FString> GetActions() const;

	// Changes a stat.
	void ApplyStatChange(EStatClass stat, int delta);

	// Called at start and end of battle.
	virtual void BeginBattle();
	virtual void EndBattle();

	UFUNCTION(BlueprintPure, Category = "Stats")
	int GetStat(EStatClass stat) const;

	UFUNCTION(BlueprintCallable, Category="Stats")
	void SetStat(EStatClass stat, int value);

	// Gets a reference (pointer!) to the stat.
	Stat * GetStatRef(EStatClass stat);

protected:
	virtual void fromXml(FXmlNode * const node);

	virtual int getEnergy() const;
	virtual void setEnergy(int energy);

private:
	Stat _hitPoints;
	Stat _maxHitPoints;
	Stat _speed;
	Stat _strength;
	Stat _dexterity;
	Stat _evasion;
	TArray<Stat> _defence;
};
