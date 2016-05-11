// (c) Smoke and Mirrors Software 2016

#pragma once

#include "Signals.h"

/**
 * 
 */
class SIGNALS_API AssetHelper
{
public:
	static AActor * SpawnActorFromNamedBlueprint(UWorld * world, FVector pos, FRotator rot, FString path, FString name);
private:
	AssetHelper();
	~AssetHelper();

};
