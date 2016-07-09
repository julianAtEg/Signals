// (c) Smoke and Mirrors Software 2016

#pragma once

#include <Engine/GameInstance.h>
#include "BattleInfo.h"
#include "SignalsInstance.generated.h"

class UHumanPlayerStats;

/**
* Game data model / instance.
*/
UCLASS(BlueprintType)
class SIGNALS_API USignalsInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	USignalsInstance(FObjectInitializer const & init);

	void Init() override;
	void Shutdown() override;

	UFUNCTION(BlueprintPure, Category = "Battle stats")
	UHumanPlayerStats * GetHumanPlayerStats(FString player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Model")
	FBattleInfo BattleInfo;

	// The monetary unit of the game, used to drive almost all the mechanics.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Model")
	int Ergs;

private:
	TMap<FString, UHumanPlayerStats *> _stats;
};
