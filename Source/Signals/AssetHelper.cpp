// (c) Smoke and Mirrors Software 2016

#include "Signals.h"
#include "AssetHelper.h"

/*static*/
AActor * AssetHelper::SpawnActorFromNamedBlueprint(UWorld * world, FVector pos, FRotator rot, FString path, FString name)
{
	if (path.Len() == 0)
	{
		return nullptr;
	}

	AActor * actor = nullptr;
	FStringAssetReference assetRef = path;
	if (assetRef.TryLoad() != nullptr)
	{
		auto obj = assetRef.ResolveObject();
		if (obj == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid BP ref (resolve) '%s'"), *path);
		}
		else
		{
			UBlueprint * gen = Cast<UBlueprint>(obj);
			if (gen == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid BP ref (cast) '%s'"), *path);
			}
			else
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.bNoFail = true;
				SpawnInfo.bRemoteOwned = false;
				if (name.Len() > 0)
				{
					SpawnInfo.Name = FName( *name );
				}

				actor = world->SpawnActor<AActor>(gen->GeneratedClass, SpawnInfo);
				if (actor == nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("Spawn failed: %s"), *assetRef.ToString());
				}
				else
				{
					FVector scale(1, 1, 1);
					FTransform transform(rot, pos, scale);
					actor->SetActorTransform(transform);
				}
			}
		}
	}

	return actor;
}