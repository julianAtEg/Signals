// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Signals.h"
#include "XmlParser.h"
#include "Ability.h"
#include "AttackClass.h"
#include "PlayerStats.generated.h"

class Random;

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

	// Determines how frequently the agent gets a turn in battle. Max out at 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Speed;

	// How hard the agent hits, max out at 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Strength;

	// How good the agent is at attacking.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Dexterity;

	// How good the agent is at avoiding attacks.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Evasion;

	// How resistant to attacks, max out at 100 (parameterised by EActionClass)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TArray<int> Defence;

	// Character development level. [0,99].
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Level;

	// Loads stats data from a file.
	void Load( FString const & filePath );

	// Computes attack, defence and healing values.
	virtual int ComputeAttack(Random * rng, int base, int levelScale, FString const & action) const;
	virtual int ComputeDefence(Random * rng, int levelScale, EAttackClass actionClass) const;
	virtual int ComputeRegain(Random * rng, int base, int levelScale, FString const & action) const;

protected:
	virtual void fromXml(FXmlNode * const node);

private:

};
