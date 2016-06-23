#pragma once

class UNpcPlayerStats;

class ResourceManager
{
public:
	ResourceManager() = default;
	~ResourceManager();
	ResourceManager(ResourceManager const & that) = delete;
	ResourceManager & operator=(ResourceManager const & that) = delete;

	void Clear();
	USoundWave * LoadAudioResource(FString const & path);
	UParticleSystem * LoadParticleSystem(FString const & path);
	UNpcPlayerStats * LoadNPCStats(FString const & name);

private:
	TMap<FString, USoundWave *> _audioResources;
	TMap<FString, UParticleSystem *> _psResources;
};
