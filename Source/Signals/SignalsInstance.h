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

	UFUNCTION(BlueprintPure, Category = "Battle stats")
	UHumanPlayerStats * GetHumanPlayerStats(FString player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Model")
	FBattleInfo BattleInfo;

private:
	TMap<FString, UHumanPlayerStats *> _stats;
};
