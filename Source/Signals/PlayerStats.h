// (c) Smoke and Mirrors Games 2016

#pragma once

#include "Signals.h"
#include "XmlParser.h"
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

	// Determines how frequently the agent gets a turn in battle. Max out at 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Speed;

	// How hard the agent hits, max out at 100.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Strength;

	// How resistant to physical hits, max out at 100
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int Defence;

	// Loads stats data from a file.
	void Load( FString const & filePath );

protected:
	virtual void fromXml(FXmlNode * const node);
};
