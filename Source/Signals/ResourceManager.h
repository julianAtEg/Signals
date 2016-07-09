#pragma once

#include "ResourceManager.generated.h"

class UNpcPlayerStats;

UCLASS()
class SIGNALS_API UResourceManager : public UObject
{
	GENERATED_BODY()
public:
	UResourceManager(const FObjectInitializer& init);
	void Clear();
	USoundWave * LoadAudioResource(FString const & path);
	UParticleSystem * LoadParticleSystem(FString const & path);
	UNpcPlayerStats * LoadNPCStats(FString const & name);

private:
	UPROPERTY()
	TMap<FString, USoundWave *> _audioResources;

	FStreamableManager _stream;
};
