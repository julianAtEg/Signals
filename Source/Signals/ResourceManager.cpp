#include "Signals.h"
#include "ResourceManager.h"
#include "NpcPlayerStats.h"

UResourceManager::UResourceManager(const FObjectInitializer& init)
: Super(init)
{

}

void UResourceManager::Clear()
{
	//for (auto & kv : _audioResources)
	//{
	//	auto sound = kv.Value;
	//	if (sound->IsValidLowLevel())
	//	{
	//		sound->ConditionalBeginDestroy();
	//	}
	//}

	_audioResources.Empty();

	//for (auto & kv : _psResources)
	//{
	//	auto particles = kv.Value;
	//	if (particles->IsValidLowLevel())
	//	{
	//		particles->RemoveFromRoot();
	//		//particles->ConditionalBeginDestroy();
	//	}
	//}

	//_psResources.Empty();

}

UNpcPlayerStats * UResourceManager::LoadNPCStats(FString const & name)
{
	auto contentFolder = FPaths::GameContentDir();
	auto dataFolder = FPaths::Combine(*contentFolder, TEXT("Data"));
	auto fileName = name + TEXT(".xml");
	auto dataFile = FPaths::Combine(*dataFolder, *fileName);
	auto stats = NewObject<UNpcPlayerStats>();
	stats->AddToRoot();
	stats->Load(dataFile);

	// TODO: cache and clone?

	return stats;
}

UParticleSystem * UResourceManager::LoadParticleSystem(FString const & name)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading particle resource '%s'"), *name);

	auto path = FString::Printf(TEXT("ParticleSystem'/Game/Particles/%s.%s'"), *name, *name);

	FStringAssetReference psRef(path);
	return Cast<UParticleSystem>(_stream.SynchronousLoad(psRef));
}

//UParticleSystem * UResourceManager::LoadParticleSystem(FString const & name)
//{
//	UE_LOG(SignalsLog, Log, TEXT("Loading particle resource '%s'"), *name);
//
//	UParticleSystem * particles;
//	auto pRes = _psResources.Find(name);
//	if (pRes == nullptr)
//	{
//		auto path = FString::Printf(TEXT("ParticleSystem'/Game/Particles/%s.%s'"), *name, *name);
//		particles = LoadObject<UParticleSystem>(nullptr, *path, nullptr, LOAD_None, nullptr);
//		if (particles != nullptr)
//		{
//			//particles->AddToRoot();
//			_psResources.Add(name, particles);
//		}
//		else
//		{
//			UE_LOG(SignalsLog, Warning, TEXT("Could not locate particle resource '%s'"), *name);
//		}
//	}
//	else
//	{
//		particles = *pRes;
//	}
//
//	return particles;
//}

USoundWave * UResourceManager::LoadAudioResource(FString const & name)
{
	UE_LOG(SignalsLog, Log, TEXT("Loading audio resource '%s'"), *name);

	USoundWave * sound;
	auto pRes = _audioResources.Find(name);
	if (pRes == nullptr)
	{
		auto path = FString::Printf(TEXT("SoundWave'/Game/Audio/%s.%s'"), *name, *name);
		sound = LoadObject<USoundWave>(nullptr, *path, nullptr, LOAD_None, nullptr);
		if (sound != nullptr)
		{
			//sound->AddToRoot();
			_audioResources.Add(name, sound);
		}
		else
		{
			UE_LOG(SignalsLog, Warning, TEXT("Could not locate audio resource '%s'"), *name);
		}
	}
	else
	{
		sound = *pRes;
	}

	return sound;
}